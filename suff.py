import networkx as nx
import matplotlib.pyplot as plt
import sys


def construct(s):
	g = nx.DiGraph()
	node_idx = 0
	g.add_node(node_idx)
	node_idx += 1
	for i in range(len(s)):
		t = s[i:]	# suffix
		print "suffix:", t
		s_idx = 0
		current = 0
		while s_idx < len(t):
			followed = None
			for c in g.neighbors(current):
				e = g.get_edge_data(current, c)
				followed = None
				if e['s'][0] == t[s_idx]:
					followed = c
					break
			if followed == None:
				g.add_edge(current, "leaf " + str(i), {'s': t[s_idx:]})
				node_idx += 1
				s_idx = len(t)
			else:
				match = False
				e = g.get_edge_data(current, followed)
				for j in range(len(e['s'])):
					if e['s'][j] == t[s_idx]:
						s_idx += 1
					else:
						g.remove_edge(current, c)

						# internal node
						internal_node = node_idx
						node_idx += 1
						g.add_edge(current, internal_node, {'s': e['s'][:j]})
						g.add_edge(internal_node, c, {'s': e['s'][j:]})

						# new leaf node
						leaf_node = node_idx
						node_idx += 1
						g.add_edge(internal_node, "leaf " + str(i), {'s': t[s_idx:]})
						match = True
						break
				if not match:
					current = followed
				else:
					break
	return g


# G = nx.DiGraph()
# 
# G.add_edges_from([
# 	(0, 1, {'s': "A"}),
# 	(1, 2, {'s': "$"}),
# 	(1, 3, {'s': "NA"}),
# 	(3, 4, {'s': "$"}),
# 	(3, 5, {'s': "NA$"}),
# 	(0, 6, {'s': "BANANA$"}),
# 	(0, 7, {'s': "NA"}),
# 	(7, 8, {'s': "$"}),
# 	(7, 9, {'s': "NA$"})
# ])
# 

G = construct(sys.argv[1])

pos = nx.graphviz_layout(G, prog='dot')
nx.draw(G, pos)
edge_labels = nx.get_edge_attributes(G,'s')
nx.draw_networkx_edge_labels(G, pos, edge_labels, font_size=15)
plt.show()
