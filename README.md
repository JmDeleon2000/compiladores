# Build
make

# Draw automatas
make draw<br/>
:warning: <a href="http://www.graphviz.org/">dot</a> is **required**

# Sample results:
ab \* ab \*<br/>
![alt text](https://github.com/JmDeleon2000/compiladores/blob/2131fa3b6de4815756cb89b17fbbc995140d3cc6/results/graph1.png?raw=true)
0? (1? )? 0 \*<br/>
![alt text](https://github.com/JmDeleon2000/compiladores/blob/2131fa3b6de4815756cb89b17fbbc995140d3cc6/results/graph2.png?raw=true)
(a\*|b\*)c<br/>
![alt text](https://github.com/JmDeleon2000/compiladores/blob/2131fa3b6de4815756cb89b17fbbc995140d3cc6/results/graph3.png?raw=true)
(b|b)\*abb(a|b)\*<br/>
![alt text](?raw=true)
(a|ε)b(a+)c?<br/>
![alt text](https://github.com/JmDeleon2000/compiladores/blob/2131fa3b6de4815756cb89b17fbbc995140d3cc6/results/graph5.png?raw=true)
(a|b)\*a(a|b)(a|b)<br/>
![alt text](https://github.com/JmDeleon2000/compiladores/blob/2131fa3b6de4815756cb89b17fbbc995140d3cc6/results/graph6.png?raw=true)