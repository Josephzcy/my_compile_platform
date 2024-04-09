"""
Caffe network visualization: draw the NetParameter protobuffer.


.. note::

    This requires pydot>=1.0.2, which is not included in requirements.txt since
    it requires graphviz and other prerequisites outside the scope of the
    Caffe.
"""

from caffe.proto import caffe_pb2

"""
pydot is not supported under python 3 and pydot2 doesn't work properly.
pydotplus works nicely (pip install pydotplus)
"""
try:
    # Try to load pydotplus
    import pydotplus as pydot
except ImportError:
    import pydot

# Internal layer and blob styles.
LAYER_STYLE_DEFAULT = {'shape': 'record',
                       'fillcolor': '#6495ED',
                       'style': 'filled'}
NEURON_LAYER_STYLE = {'shape': 'record',
                      'fillcolor': '#90EE90',
                      'style': 'filled'}
BLOB_STYLE = {'shape': 'octagon',
              'fillcolor': '#E0E0E0',
              'style': 'filled'}


def get_pooling_types_dict():
    """Get dictionary mapping pooling type number to type name
    """
    desc = caffe_pb2.PoolingParameter.PoolMethod.DESCRIPTOR
    d = {}
    for k, v in desc.values_by_name.items():
        d[v.number] = k
    return d


def get_edge_label(layer):
    """Define edge label based on layer type.
    """

    if layer.type == 'Data':
        edge_label = 'Batch ' + str(layer.data_param.batch_size)
    elif layer.type == 'Convolution' or layer.type == 'Deconvolution':
        edge_label = str(layer.convolution_param.num_output)
    elif layer.type == 'InnerProduct':
        edge_label = str(layer.inner_product_param.num_output)
    else:
        edge_label = '""'

    return edge_label


def get_layer_label(layer, rankdir):
    """Define node label based on layer type.

    Parameters
    ----------
    layer : ?
    rankdir : {'LR', 'TB', 'BT'}
        Direction of graph layout.

    Returns
    -------
    string :
        A label for the current layer
    """

    if rankdir in ('TB', 'BT'):
        # If graph orientation is vertical, horizontal space is free and
        # vertical space is not; separate words with spaces
        separator = ' '
    else:
        # If graph orientation is horizontal, vertical space is free and
        # horizontal space is not; separate words with newlines
        separator = '\\n'

    if layer.type == 'Convolution' or layer.type == 'Deconvolution':
        # Outer double quotes needed or else colon characters don't parse
        # properly
        node_label = '"%s%s(%s)%skernel size: %d%sstride: %d%spad: %d%sgroup: %d%sdialtion: %d"' %\
                     (layer.name,
                      separator,
                      layer.type,
                      separator,
                      layer.convolution_param.kernel_size[0] if len(layer.convolution_param.kernel_size._values) else 1,
                      separator,
                      layer.convolution_param.stride[0] if len(layer.convolution_param.stride._values) else 1,
                      separator,
                      layer.convolution_param.pad[0] if len(layer.convolution_param.pad._values) else 0,
                      separator,
                      layer.convolution_param.group,
                      separator,
                      layer.convolution_param.dilation[0] if len(layer.convolution_param.dilation._values) else 1
                      )
    elif layer.type == 'Pooling':
        pooling_types_dict = get_pooling_types_dict()
        node_label = '"%s%s(%s %s)%skernel size: %d%sstride: %d%spad: %d"' %\
                     (layer.name,
                      separator,
                      pooling_types_dict[layer.pooling_param.pool],
                      layer.type,
                      separator,
                      layer.pooling_param.kernel_size,
                      separator,
                      layer.pooling_param.stride,
                      separator,
                      layer.pooling_param.pad)
    else:
        node_label = '"%s%s(%s)"' % (layer.name, separator, layer.type)
    return node_label


def choose_color_by_layertype(layertype):
    """Define colors for nodes based on the layer type.
    """
    color = '#6495ED'  # Default
    if layertype == 'Convolution' or layertype == 'Deconvolution':
        color = '#FF5050'
    elif layertype == 'Pooling':
        color = '#FF9900'
    elif layertype == 'InnerProduct':
        color = '#CC33FF'
    return color


def get_pydot_graph(caffe_net, complexity_dict, model_size_dict, shape_dict, profiler_dict, rankdir, label_edges=True):
    """Create a data structure which represents the `caffe_net`.

    Parameters
    ----------
    caffe_net : object
    rankdir : {'LR', 'TB', 'BT'}
        Direction of graph layout.
    label_edges : boolean, optional
        Label the edges (default is True).

    Returns
    -------
    pydot graph object
    """
    total_complexity = 0
    for item in complexity_dict.keys():
        total_complexity += complexity_dict[item]
    for item in complexity_dict.keys():
        complexity_dict[item] /= float(total_complexity)
    total_model_size = 0
    for item in model_size_dict.keys():
        total_model_size += model_size_dict[item]

    pydot_graph = pydot.Dot(caffe_net.name if caffe_net.name else 'Net',
                            graph_type='digraph',
                            rankdir=rankdir)
    pydot_nodes = {}
    pydot_edges = []
    for layer in caffe_net.layer:
        node_label = get_layer_label(layer, rankdir)

        node_name = "%s_%s" % (layer.name, layer.type)
        if (len(layer.bottom) == 0): #input
            node_label = node_label + ("\nsize:%d" % total_model_size)
            node_label = node_label + ("\ncomplex:%d" % total_complexity)
            if profiler_dict is not None:
               node_label = node_label + ("\ntime:%f" % profiler_dict["###all_layers_processing_time###"])
            pydot_nodes[node_name] = pydot.Node(node_label, **NEURON_LAYER_STYLE)
        else:
            if layer.name in model_size_dict.keys():
                node_label = node_label + ("\nsize:%d" % model_size_dict[layer.name])
            if layer.name in model_size_dict.keys():
                node_label = node_label + ("\ncomplex:%.3f" % complexity_dict[layer.name])
            if profiler_dict is not None and layer.name in profiler_dict.keys():
                node_label = node_label + ("\ntime:%.3f" % profiler_dict[layer.name])
                if layer.name in complexity_dict.keys():
                    efficiency = total_complexity * complexity_dict[layer.name] / (profiler_dict[layer.name] * profiler_dict["###all_layers_processing_time###"] + 0.000000001) / 1000000.0
                    node_label = node_label + ("\nefficiency:%.3f" % efficiency)

            if (len(layer.bottom) == 1 and len(layer.top) == 1 and layer.bottom[0] == layer.top[0]):
                # We have an in-place neuron layer.
                pydot_nodes[node_name] = pydot.Node(node_label, **NEURON_LAYER_STYLE)
            else:
                layer_style = LAYER_STYLE_DEFAULT
                layer_style['fillcolor'] = choose_color_by_layertype(layer.type)
                pydot_nodes[node_name] = pydot.Node(node_label, **layer_style)

        for bottom_blob in layer.bottom:
            shape = shape_dict[bottom_blob]
            pydot_nodes[bottom_blob + '_blob'] = pydot.Node(('%s\n(%d, %d, %d)' % (bottom_blob, shape[0], shape[1], shape[2] )), **BLOB_STYLE)
            edge_label = '""'
            pydot_edges.append({'src': bottom_blob + '_blob',
                                'dst': node_name,
                                'label': edge_label})
        for top_blob in layer.top:
            shape = shape_dict[top_blob]
            pydot_nodes[top_blob + '_blob'] = pydot.Node('%s\n(%d, %d, %d)' % (top_blob, shape[0], shape[1], shape[2]))

            if label_edges:
                edge_label = get_edge_label(layer)
            else:
                edge_label = '""'
            pydot_edges.append({'src': node_name,
                                'dst': top_blob + '_blob',
                                'label': edge_label})
    # Now, add the nodes and edges to the graph.
    for node in pydot_nodes.values():
        pydot_graph.add_node(node)
    for edge in pydot_edges:
        pydot_graph.add_edge(
            pydot.Edge(pydot_nodes[edge['src']],
                       pydot_nodes[edge['dst']],
                       label=edge['label']))
    return pydot_graph


def draw_net(caffe_net, complexity_dict, model_size_dict, shape_dict, profiler_dict, rankdir, ext='png'):
    """Draws a caffe net and returns the image string encoded using the given
    extension.

    Parameters
    ----------
    caffe_net : a caffe.proto.caffe_pb2.NetParameter protocol buffer.
    ext : string, optional
        The image extension (the default is 'png').

    Returns
    -------
    string :
        Postscript representation of the graph.
    """
    return get_pydot_graph(caffe_net, complexity_dict, model_size_dict, shape_dict, profiler_dict, rankdir).create(format=ext)


def draw_net_to_file(caffe_net, filename, complexity_dict, model_size_dict, shape_dict, profiler_dict, rankdir='LR'):
    """Draws a caffe net, and saves it to file using the format given as the
    file extension. Use '.raw' to output raw text that you can manually feed
    to graphviz to draw graphs.

    Parameters
    ----------
    caffe_net : a caffe.proto.caffe_pb2.NetParameter protocol buffer.
    filename : string
        The path to a file where the networks visualization will be stored.
    rankdir : {'LR', 'TB', 'BT'}
        Direction of graph layout.
    """
    ext = filename[filename.rfind('.')+1:]
    with open(filename, 'wb') as fid:
        fid.write(draw_net(caffe_net, complexity_dict, model_size_dict, shape_dict, profiler_dict, rankdir, ext))
