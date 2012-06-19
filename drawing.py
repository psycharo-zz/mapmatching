import matplotlib.pyplot as plt
import numpy as np
import math


if __name__ == '__main__':  

  # draw the input_01.txt or sth
  input = open('./input/input_01.txt')
  x1 = []
  y1 = []
  for line in input:
    coord = line.split(',')
    x1.append(float(coord[1]))
    y1.append(float(coord[2]))
    
  plt.plot(x1,y1,'black', linewidth = 5)
  
  # draw the algo. output
  output = open('./testoutput.txt')
  matched_edges = dict()
  for line in output:
    coord = line.split(',')
    matched_edges[int(coord[1])] = float(coord[2])
  
  # draw the graph, in the rectangle(top, bottom, left, right)
  top = max(y1)
  bottom = min(y1)
  left = min(x1)
  right = max(x1)
  
  print top, bottom, left, right
  
  graph = open('WA_EdgeGeometry.txt')
  
  for line in graph:
    linesplits = line.split('^')
    edgeid = int(linesplits[0])
    coord = linesplits[4:]
    #print coord , '\n'
    x = []
    y = []
    tag = False
    for i in range(0,len(coord),2):
      p1 = float(coord[i])
      p2 = float(coord[i+1])
      if ((left <= p1 <= right) and (bottom <= p2 <= top)): 
	tag = True
	x.append(p1)
	y.append(p2)
    if tag is True:
      plt.plot(x, y, 'b')
    if edgeid in matched_edges.keys():
      plt.plot(x, y, 'r', linewidth = 5*matched_edges[edgeid])
    
  
  # restrict the xaxis and yaxis
  plt.xlim(left, right)
  plt.ylim(bottom, top)
  plt.show()