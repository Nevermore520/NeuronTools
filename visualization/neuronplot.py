#!/usr/bin/env python3

from collections import Counter
from random import shuffle
import argparse
import itertools
import pathlib
import string
import warnings

from scipy.cluster import hierarchy
from scipy.spatial.distance import squareform
from sklearn.manifold import spectral_embedding
import networkx as nx
import numpy as np

from bokeh.models import (
    ColumnDataSource, TableColumn, DataTable, VBox, HBox, CustomJS, Circle,
    HoverTool, Toggle, TapTool, BoxSelectTool
    )
from bokeh.palettes import Spectral11
from bokeh.plotting import figure, show, output_file


def read_distance_matrix(path):
    with open(path) as distance_file:
        m = 0
        for line in distance_file:
            m += 1
        n = len(line.strip().split())

    if m != n:
        skiprows = 1
    else:
        skiprows = 0

    return np.loadtxt(path, skiprows=skiprows)


def read_labels(path):
    labels = []
    identifiers = []
    with open(path) as labels_file:
        for i, line in enumerate(labels_file):
            parts = line.strip().split(None, 1)[1]
            parts = parts.strip().split('\\')
            label = '_'.join(parts[:-1])
            identifier = parts[-1]
            labels.append(label)
            identifiers.append(identifier)

    return labels, identifiers


def linkage_matrix_to_networkx(linkage):
    """Converts a linkage matrix to a networkx tree."""
    n = len(linkage) + 1

    tree = nx.DiGraph()
    tree.add_nodes_from(range(n), value=0.0)

    node = itertools.count(start=n)

    for u, v, height, _ in linkage:
        parent = next(node)
        tree.add_node(parent, value=height)
        tree.add_edge(parent, int(u), members={})
        tree.add_edge(parent, int(v), members={})
        
    tree.graph['root'] = len(tree) - 1
    return tree


def dendrogram(distances, method='average'):
    """Given a distance matrix, computes the dendrogram as a networkx tree."""
    linkage = hierarchy.linkage(squareform(distances), method=method)
    return linkage_matrix_to_networkx(linkage)


def _generate_nodes(tree, predicate, source=None):
    """Generate nodes in a DFS."""
    if source is None:
        source = tree.graph['root']
    for node in nx.dfs_postorder_nodes(tree, source=source):
        if predicate(tree, node):
            yield node


def leaf_nodes(tree, source=None):
    """Generate leaf nodes in a DFS."""
    def predicate(t, node): return t.out_degree(node) == 0 
    yield from _generate_nodes(tree, predicate, source)
            

def internal_nodes(tree, source=None):
    """Generate leaf nodes in a DFS."""
    def predicate(t, node): return t.out_degree(node) > 0 
    yield from _generate_nodes(tree, predicate, source)
            

def _assign_slots(tree):
    root = len(tree) - 1
    for node in tree:
        tree.node[node]['n_desc'] = len(list(leaf_nodes(tree, source=node)))
    
    tree.node[root]['allotment'] = 0
        
    for node in nx.dfs_preorder_nodes(tree, source=root):
        children = tree.successors(node)
        children = sorted(children, key=lambda c: tree.node[c]['n_desc'])
        allotment = tree.node[node]['allotment']
        offset = 0
        for child in children:
            tree.node[child]['allotment'] = allotment + offset
            offset += tree.node[child]['n_desc']
            
        if tree.out_degree(node) == 0:
            tree.node[node]['x'] = tree.node[node]['allotment']
            

def _assign_internal(tree):
    for node in internal_nodes(tree):
        children = list(tree.successors(node))
        mu = sum(tree.node[l]['x'] for l in children) / len(children)
        tree.node[node]['x'] = mu


def embed_dendrogram(tree):
    """Computes the locations of the nodes of a dendrogram in the plot."""
    _assign_slots(tree)
    _assign_internal(tree)
        

def dendrogram_edges(tree):
    scale = max(tree.node[u]['value'] for u in tree)
    edges = []
    for parent in internal_nodes(tree):
        children_x = [tree.node[u]['x'] for u in tree.successors(parent)]
        x_min = min(children_x)
        x_max = max(children_x)
        y = tree.node[parent]['value']
        edges.append((x_min, y, x_max, y))
        
        for child in tree.successors(parent):
            if tree.out_degree(child) == 0:
                y_child = max(y - (2/100.)*scale, 0)
            else:
                y_child = tree.node[child]['value']
            x = tree.node[child]['x']
            edges.append((x, y_child, x, y))
            
    return edges


def embed(distances):
    heat = np.exp(-distances / distances.std())
    return spectral_embedding(heat, n_components=2)


def normalize(s):
    for char in ('-', ' ', '+', '-'):
        s = s.replace(char, '_')
    return s


def make_key(label, identifier):
    return normalize(label) + '_' + normalize(identifier)


def make_neurons_source(tree, distances):
    n = len(list(leaf_nodes(tree)))
    slots = []
    for node in range(n):
        slots.append(tree.node[node]['x'])

    x, y = embed(distances).T
        
    return ColumnDataSource({
        'slot': slots, 
        'height': [0]*len(slots), 
        'x': x, 'y': y,
        'dendrogram_size': [1]*len(slots), 
        'dendrogram_alpha': [.3]*len(slots),
        'color': ['black']*len(slots)
        })


def make_edges_source(tree):
    edges = dendrogram_edges(tree)
    x0, y0, x1, y1 = zip(*edges)
    return ColumnDataSource({'x0': x0, 'y0': y0, 'x1': x1, 'y1': y1})


def make_legend_source():
    return (
        ColumnDataSource({'x': [], 'y': [], 'labels': []}),
        ColumnDataSource({'x': [], 'y': [], 'colors': []})
        )


def make_labels_source(neurons_source, labels, identifiers, colormap):
    neurons_source.add(labels, name='label')
    neurons_source.add(identifiers, name='identifier')

    counter = Counter(labels)
    items = sorted(counter.items(), key=lambda x: x[1], reverse=True)
    names, counts = zip(*items)
    shuffle(colormap)
    colors = list(itertools.islice(itertools.cycle(colormap), len(names)))
    return ColumnDataSource({
        'labels': names, 
        'counts': counts, 
        'colors': colors
        })


def make_cell_sources(cell_data, labels, identifiers):
    keys = [make_key(l, i) for l, i in zip(labels, identifiers)]
    cell_sources = {}

    for key in keys:
        try:
            tree = cell_data[key]
        except KeyError:
            cell_sources['data_' + key] = ColumnDataSource({'x0':[], 'y0':[],
                'x1': [], 'y1': []})
            err = 'Could not load key "{}".'.format(key)
            warnings.warn(err)
            continue

        x, y, z = tree[:, 2:5].T
        edges = np.asarray(tree[1:,[0,-1]], dtype=int)
        x0, y0 = x[edges[:,0]-1], y[edges[:,0]-1]
        x1, y1 = x[edges[:,1]-1], y[edges[:,1]-1]
        cell_sources['data_' + key] = ColumnDataSource({
            'x0': x0, 'y0': y0, 'x1': x1, 'y1': y1
            })

    edges = ColumnDataSource({'x0': [], 'y0': [], 'x1': [], 'y1': []})
    cell_sources['tree'] = edges

    return cell_sources


def make_labels_callback(labels_source, neurons_source, legend_source, 
                         legend_circles_source, y):
    args = {
        'neurons': neurons_source, 
        'legend': legend_source, 
        'legend_circles': legend_circles_source
    }
    template = string.Template("""
        console.log("hello")
        var label_inds = cb_obj.get('selected')['1d'].indices;
        var label_data = cb_obj.get('data');
        
        var neurons_inds = neurons.get('selected')['1d'].indices;
        var neurons_data = neurons.get('data');
        
        var legend_data = legend.get('data')
        var legend_circles_data = legend_circles.get('data')

        var selected = [];
        for (i=0; i < neurons_data['label'].length; ++i) {
            var match = -1;
            for (j=0; j < label_inds.length; ++j) {
                if (neurons_data['label'][i] === label_data['labels'][label_inds[j]]) {
                    selected.push(i);
                    match = j;
                }
            }
            
            if (match >= 0) {
                neurons_data['dendrogram_size'][i] = 10;
                neurons_data['color'][i] = label_data['colors'][label_inds[match]];
                neurons_data['dendrogram_alpha'][i] = .5;
            } else {
                neurons_data['dendrogram_size'][i] = 1;
                neurons_data['color'][i] = 'black'
                neurons_data['dendrogram_alpha'][i] = .1;
            }
        }

        var legend_y = $y;
        var legend_scale = legend_y / 25;
        legend_y += legend_scale;
        
        legend_data['labels'].length = 0;
        legend_data['x'].length = 0;
        legend_data['y'].length = 0;
        legend_circles_data['colors'].length = 0;
        legend_circles_data['x'].length = 0;
        legend_circles_data['y'].length = 0;
        for (j=0; j < label_inds.length; ++j) {
            legend_data['labels'].push(label_data['labels'][label_inds[j]]);
            legend_data['x'].push(.5);
            legend_data['y'].push(legend_y + legend_scale * j);
            legend_circles_data['x'].push(-3);
            legend_circles_data['y'].push(legend_y + legend_scale * j);
            legend_circles_data['colors'].push(label_data['colors'][label_inds[j]]);
        }
        legend.trigger('change');
        legend_circles.trigger('change');

        neurons.get('selected')['1d'].indices = selected;
        neurons.trigger('change')
    """)
    code = template.substitute(y=y)
    labels_source.callback = CustomJS(args=args, code=code)


def make_table_widget(labels_source):
    columns = [
        TableColumn(field='labels', title='Neuron'), 
        TableColumn(field='counts', title='Count')
        ]

    return DataTable(
        source=labels_source, columns=columns, width=300, row_headers=False)


def make_dendrogram(neurons_source, edges_source, legend_source, 
                    legend_circles_source):
    dendrogram = figure(plot_width=1200)

    dendrogram.circle(source=neurons_source, x='slot', y='height',
            color='color', size='dendrogram_size', alpha='dendrogram_alpha',
            line_color='black')

    dendrogram.text(source=legend_source, x='x', y='y', text='labels')
    dendrogram.circle(source=legend_circles_source, x='x', y='y',
            color='colors', size=10)

    dendrogram.segment(source=edges_source, x0='x0', y0='y0', x1='x1', y1='y1',
            color='black', alpha=.7)

    dendrogram.xaxis.major_tick_line_color = None
    dendrogram.xaxis.minor_tick_line_color = None
    dendrogram.xaxis.major_label_text_color = None

    dendrogram.add_tools(BoxSelectTool())

    return dendrogram


def make_embedding(neurons_source, cell_sources):
    embedding = figure()

    data = neurons_source.data
    scale = max(data['x'].max(), data['y'].max())

    embedding.circle(source=neurons_source, x='x', y='y', color='color',
            radius=(1./100)*scale, alpha=1, line_color='black')

    hover = HoverTool(tooltips=[
        ('Label', '@label'), 
        ('Identifier', '@identifier')
        ])
    embedding.add_tools(hover)

    tap = TapTool(callback=CustomJS(args=cell_sources, code="""
        var inds = cb_obj.get('selected')['1d'].indices
        var scatter_data = cb_obj.get('data')

        var old_data = tree.get('data')


        if (inds.length > 0) {
            var label = scatter_data['label'][inds[0]]
            var identifier = scatter_data['identifier'][inds[0]]
            var key = "data_" + label + "_" + identifier
            key = key.replace(/-/g, "_")
            key = key.replace(/ /g, "_")
            eval('data = ' + key)
            data = data.get('data')

            old_data['x0'] = data['x0']
            old_data['y0'] = data['y0']
            old_data['x1'] = data['x1']
            old_data['y1'] = data['y1']


            tree.trigger('change')
        }
        """))
    embedding.add_tools(tap)

    return embedding


def make_tree(edges):
    tree = figure()
    tree.segment(source=edges, x0='x0', y0='y0', x1='x1', y1='y1',
                 color='black')
    tree.grid.grid_line_color = None
    tree.axis.major_label_text_color = None
    tree.axis.minor_tick_line_color = None
    tree.axis.major_tick_line_color = None
    tree.title = 'neuron structure'
    return tree


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('data', help='Path to the data archive.')
    parser.add_argument('distances', help='Path to distance matrix.')
    parser.add_argument('labels', help='Path to labels file.')
    parser.add_argument('--out', default=None)
    args = parser.parse_args()

    if args.out is None:
        path = pathlib.Path(args.distances)
        args.out = 'plot_' + path.stem

    data = np.load(args.data)
    labels, identifiers = read_labels(args.labels)

    distances = read_distance_matrix(args.distances)
    tree = dendrogram(distances)
    embed_dendrogram(tree)

    colormap = Spectral11.copy()
    colormap.remove('#ffffbf')

    neurons_source = make_neurons_source(tree, distances)
    edges_source = make_edges_source(tree)
    legend_source, legend_circles_source = make_legend_source()
    labels_source = make_labels_source(neurons_source, labels, identifiers, 
                                       colormap)
    cell_sources = make_cell_sources(data, labels, identifiers)

    labels_y = max(tree.node[u]['value'] for u in tree)

    make_labels_callback(labels_source, neurons_source, legend_source, 
                         legend_circles_source, labels_y)

    table = make_table_widget(labels_source)
    dendrogram = make_dendrogram(neurons_source, edges_source, legend_source, 
                                 legend_circles_source)

    embedding =  make_embedding(neurons_source, cell_sources)

    tree = make_tree(cell_sources['tree'])

    output_file(args.out)
    layout = HBox(table, VBox(dendrogram, HBox(embedding, tree)))
    show(layout)
