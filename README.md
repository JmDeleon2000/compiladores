# Build
make

# Draw automatas
make draw<br/>
:warning: <a href="http://www.graphviz.org/">dot</a> is **required**

# Sample results:
ab \* ab \*<br/>
![alt text](https://github.com/JmDeleon2000/compiladores/blob/main/results/graph1.png?raw=true)<br/>
0? (1? )? 0 \*<br/>
![alt text](https://github.com/JmDeleon2000/compiladores/blob/main/results/graph2.png?raw=true)<br/>
(a\*|b\*)c<br/>
![alt text](https://github.com/JmDeleon2000/compiladores/blob/main/results/graph3.png?raw=true)<br/>
(b|b)\*abb(a|b)\*<br/>
![alt text](https://github.com/JmDeleon2000/compiladores/blob/main/results/graph4.png?raw=true)<br/>
(a|ε)b(a+)c?<br/>
![alt text](https://github.com/JmDeleon2000/compiladores/blob/main/results/graph5.png?raw=true)<br/>
(a|b)\*a(a|b)(a|b)<br/>
![alt text](https://github.com/JmDeleon2000/compiladores/blob/main/results/graph6.png?raw=true)
