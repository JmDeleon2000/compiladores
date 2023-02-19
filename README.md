# Build
make

# Draw automatas
make draw<br/>
:warning: <a href="http://www.graphviz.org/">dot</a> is **required**

# Sample results:
ab \* ab \*<br/>
![alt text](https://github.com/JmDeleon2000/compiladores/blob/2131fa3b6de4815756cb89b17fbbc995140d3cc6/results/graph1.png?raw=true)<br/>
0? (1? )? 0 \*<br/>
![alt text](https://github.com/JmDeleon2000/compiladores/blob/2131fa3b6de4815756cb89b17fbbc995140d3cc6/results/graph2.png?raw=true)<br/>
(a\*|b\*)c<br/>
![alt text](https://github.com/JmDeleon2000/compiladores/blob/2131fa3b6de4815756cb89b17fbbc995140d3cc6/results/graph3.png?raw=true)<br/>
(b|b)\*abb(a|b)\*<br/>
![alt text](https://github.com/JmDeleon2000/compiladores/blob/2e4bafb8e807eee29ecb95efca68f9201338c3d2/results/graph4.png?raw=true)<br/>
(a|ε)b(a+)c?<br/>
![alt text](https://github.com/JmDeleon2000/compiladores/blob/2131fa3b6de4815756cb89b17fbbc995140d3cc6/results/graph5.png?raw=true)<br/>
(a|b)\*a(a|b)(a|b)<br/>
![alt text](https://github.com/JmDeleon2000/compiladores/blob/2131fa3b6de4815756cb89b17fbbc995140d3cc6/results/graph6.png?raw=true)
