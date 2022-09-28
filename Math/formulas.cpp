
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
                        F_{2k} &= F_k \left( 2F_{k+1} - F_{
  k} \right). \\
                        F_{2k+1} &= F_{k+1}^2 + F_{k}^2.
\end{array}$$

$$x^{
  n}\equiv x^{
  \phi(m) + [n \bmod \phi(m)]} \mod m  , n>=log2(m)$$


(p-1)! mod p = -1


NTT root Formula:
kth root of mod = (primitive_root)^((mod-1)/ gcd(mod-1,1<<n) )

<!-- 	\subsection{
  Labeled unrooted trees
}
-- > Labeled unrooted trees :
\#on $n$ vertices : $n ^
{ n - 2 } $ \\

\#on $k$ existing trees of size $n_i$
    : $n_1n_2\cdots n_k n ^
{ k - 2 } $ \\

\#with degrees $d_i$ : $(n - 2) ! / ((d_1 - 1) ! \cdots(d_n - 1) !) $

                        with k components
          -
          k * (n ^ (n - k - 1))

                  Kirchoff thm
          - number of spanning trees of any given graph =
    Cofactor_of_any_element(Degree_matrix - Adjacecy_matrix)

        Total number of partitions of $n$ distinct elements.$B(n) = $ $1,
                                                  1, 2, 5, 15, 52, 203, 877,
                                                  4140,
                                                  21147, \dots$.For $p$ prime,
		\[B(p ^ m + n)\equiv mB(n) + B(n + 1) \pmod { p } \]
