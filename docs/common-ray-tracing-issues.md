# Common Issues in Ray Tracing

## 1. Lambertian Reflection
Lambertian reflection describes a surface that appears equally bright from all viewing directions—an "ideal matte" surface. The light scattered follows **Lambert's Cosine Law**: the intensity of reflected light is proportional to the cosine of the angle between the surface normal ($\vec{n}$) and the incident light direction.

For a ray tracer, we simulate this by picking a random unit vector on the hemisphere defined by the surface normal.
- **Implementation:** To get a Lambertian distribution, we add a random unit vector to the surface normal:
  $$\vec{s} = \vec{n} + \vec{r}$$
  where $\vec{r}$ is a random unit vector. If $\vec{s}$ results in a zero vector (due to $\vec{r}$ being exactly $-\vec{n}$), we default to the normal $\vec{n}$ to avoid division errors.

### Why `n + r` Never Points Inward

Both $\vec{n}$ and $\vec{r}$ are unit vectors (length 1). What matters is the angle between them. The dot product tells us whether the result $\vec{s}$ points above or below the surface:

$$\vec{s} \cdot \vec{n} = (\vec{n} + \vec{r}) \cdot \vec{n} = \vec{n} \cdot \vec{n} + \vec{r} \cdot \vec{n} = 1 + \cos\theta$$

Since $\cos\theta \geq -1$, we get $\vec{s} \cdot \vec{n} \geq 0$. The result is always zero or positive—meaning $\vec{s}$ is always perpendicular to or above the surface. It only hits zero when $\vec{r}$ points exactly opposite to $\vec{n}$ ($\theta = 180°$), which is the degenerate zero-vector case the code handles separately.

### Concrete Example

Let $\vec{n} = (0, 1, 0)$ (surface normal pointing up). Consider different random vectors:

| $\vec{r}$ | $\theta$ from $\vec{n}$ | $\vec{s} = \vec{n} + \vec{r}$ | $\vec{s} \cdot \vec{n}$ |
|---|---|---|---|
| $(0, 1, 0)$ | 0° (same direction) | $(0, 2, 0)$ | 2.0 — straight up |
| $(0.6, 0.8, 0)$ | 37° | $(0.6, 1.8, 0)$ | 1.8 — tilted but upward |
| $(1, 0, 0)$ | 90° (perpendicular) | $(1, 1, 0)$ | 1.0 — grazing along surface |
| $(-0.6, -0.8, 0)$ | 143° | $(-0.6, 0.2, 0)$ | 0.2 — barely above surface |
| $(0, -1, 0)$ | 180° (opposite) | $(0, 0, 0)$ | 0.0 — zero vector, fallback to $\vec{n}$ |

As $\vec{r}$ swings from aligned with $\vec{n}$ to opposite, the result $\vec{s}$ smoothly transitions from pointing straight up, to grazing the horizon, to vanishing. It never points below the surface. After normalization, this produces the cosine-weighted distribution: most directions cluster near the normal, fewer near the horizon—exactly Lambert's law.

## 2. Shadow Acne
Shadow acne occurs due to floating-point precision limitations in ray-surface intersections.

### The Problem
When a ray hits a surface at point $P$, it calculates a new ray to scatter. Due to precision errors, the origin of this "scattered" ray might be calculated as slightly *inside* the geometry rather than exactly on the surface. This causes the ray to immediately intersect the surface it just left.

### The Visual Result
The surface appears covered in dark, noisy moiré patterns or "speckles" of black, as the surface is effectively "shadowing itself."

### The Solution: Ray Bias
We introduce a small **epsilon ($\epsilon$) offset** along the normal vector when spawning new rays. Instead of starting the ray at $P$, we start it at $P + \epsilon \cdot \vec{n}$.
- A small value like $10^{-4}$ or $10^{-5}$ is typically sufficient to push the origin clearly outside the geometry without introducing visible gaps.

## 3. Recursion Ray Entering a Different Sphere

### The Problem
When a ray scatters recursively using a new random unit vector direction, the scattered ray can intersect a *different* sphere in the scene—even one that the original ray never hit. This is not a precision artifact; it is a geometric reality of how random scattering works.

Consider a ray hitting sphere A at point $P$, with surface normal $\vec{n}$. The scattered direction is computed as:
$$\vec{d} = \vec{n} + \vec{r}$$
where $\vec{r}$ is a random unit vector. The new ray origin is offset to $P + \epsilon \cdot \vec{n}$.

The issue: $\vec{d}$ can point in *any* direction within the hemisphere defined by $\vec{n}$. If another sphere B happens to lie in that direction—even partially behind or to the side of the surface—the scattered ray will intersect it. This is physically correct behavior for diffuse (Lambertian) reflection: light bounces in all outward directions.

However, problems arise in practice:

1. **False color bleeding / light leaks:** A scattered ray from a bright surface can reach a nearby surface that should be in shadow, causing incorrect illumination. This is especially visible when spheres are close together—the scattered ray from one sphere's surface can easily reach the surface of a neighboring sphere.

2. **Unexpected self-intersection with nearby geometry:** Even with the epsilon bias pushing the origin outward along $\vec{n}$, if two spheres are nearly touching or overlapping, the biased origin on sphere A may still be *inside* sphere B. The epsilon offset only guarantees the origin is outside the surface it was computed on—it says nothing about other objects in the scene.

3. **Performance impact:** Each recursive bounce can spawn rays that hit arbitrary geometry, causing the ray tree to explore many more surfaces than expected. In scenes with many nearby objects, this leads to a combinatorial explosion of intersection tests.

### Concrete Example
Imagine two spheres of radius 1.0, centered at $(0, 0, 0)$ and $(1.9, 0, 0)$. They are nearly touching (gap of 0.1). A ray hits the rightmost point of the left sphere at $P = (1.0, 0, 0)$, with normal $\vec{n} = (1, 0, 0)$. The scattered direction $\vec{d} = \vec{n} + \vec{r}$ could easily point toward the right sphere. Even with an epsilon offset of $10^{-4}$, the new origin $(1.0001, 0, 0)$ is still only 0.8999 units from the right sphere's center—well within its radius of 1.0. The ray enters the second sphere immediately.

### The Solution
There is no single fix—this is inherent to how ray tracing works. However, several strategies mitigate the issue:

- **Proper epsilon bias:** Always offset the ray origin along the normal. This prevents self-intersection with the *same* surface, though it cannot prevent intersection with *other* nearby geometry.
- **Minimum ray distance (t_min):** When testing intersections for the scattered ray, set a small minimum $t$ value (e.g., $t_{min} = 0.001$) to discard intersections that are too close to the origin. This filters out hits that occur within the epsilon neighborhood.
- **Scene design awareness:** Keep sufficient spacing between objects. If objects are too close, even correct ray tracing will produce artifacts because the epsilon bias cannot cover the gap.
- **Max recursion depth:** Cap the number of bounces to prevent runaway recursion when rays keep finding new surfaces to hit.
