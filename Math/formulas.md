
* Cassini's identity:
  
$$F_{n-1} F_{n+1} - F_n^2 = (-1)^n$$

* The "addition" rule:
  
$$F_{n+k} = F_k F_{n+1} + F_{k-1} F_n$$

* Applying the previous identity to the case $k = n$, we get:
  
$$F_{2n} = F_n (F_{n+1} + F_{n-1})$$

* From this we can prove by induction that for any positive integer $k$,  $F_{nk}$ is multiple of $F_n$.

* The inverse is also true: if $F_m$ is multiple of $F_n$, then $m$ is multiple of $n$.

* GCD identity:
  
$$GCD(F_m, F_n) = F_{GCD(m, n)}$$

$$ \begin{array}{rll}
                        F_{2k} &= F_k \left( 2F_{k+1} - F_{k} \right). \\
                        F_{2k+1} &= F_{k+1}^2 + F_{k}^2.
\end{array}$$

$$x^{n}\equiv x^{\phi(m)+[n \bmod \phi(m)]} \mod m  , n>=log2(m)$$
