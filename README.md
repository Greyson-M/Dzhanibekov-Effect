<script type="text/x-mathjax-config">
  MathJax.Hub.Config({
    tex2jax: {
      inlineMath: [ ['$','$'], ["\\(","\\)"] ],
      processEscapes: true
    }
  });
</script>
    
<script type="text/javascript"
        src="https://cdn.mathjax.org/mathjax/latest/MathJax.js?config=TeX-AMS-MML_HTMLorMML">
</script>

# Dzhanibekov Effect

The Dzhanibekov effect is a phenomenon in which a rotating rigid body exhibits unexpected motion due to its moments of inertia. The effect is a result of the intermediate axis theorem, which states that a rigid body rotating about its intermediate axis is unstable.

![Dzhanibekov Effect](2to1top.gif)

This effect comes from Euler's equations of motion for a rigid body:

$$
I \dot{\omega} + \omega \times (I\omega) = 0
$$

This numerical simulation was rendered using a custom 3D rendering engine written in C++. The effect is most notable when the body is not subjected to gravity or other external forces, as shown in the simulation.
