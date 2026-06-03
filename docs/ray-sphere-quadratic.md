# Ray-Sphere Quadratic Intuition

A ray gives a point in space for each value of `t`:

$$
P(t) = O + tD
$$

Where:

- `O` = ray origin
- `D` = ray direction
- `t` = distance/scale along the ray

A sphere is the set of every point whose distance from its center equals the radius:

$$
(x - C_x)^2 + (y - C_y)^2 + (z - C_z)^2 = r^2
$$

Where:

- `C` = sphere center
- `r` = sphere radius
- `P = (x, y, z)` = point being tested

The same equation is cleaner in vector form:

$$
(P - C) \cdot (P - C) = r^2
$$

Why? Because dotting a vector with itself gives its squared length:

$$
v \cdot v = \lVert v \rVert^2
$$

So `(P - C) · (P - C)` means:

"the squared distance from point `P` to the sphere center `C`."

## Substitute the ray into the sphere

The ray tells us that every point on the ray is:

$$
P(t) = O + tD
$$

Put that into the sphere equation:

$$
(O + tD - C) \cdot (O + tD - C) = r^2
$$

Define a helper vector:

$$
oc = O - C
$$

Then the equation becomes:

$$
(oc + tD) \cdot (oc + tD) = r^2
$$

Expand the dot product:

$$
(D \cdot D)t^2 + 2(oc \cdot D)t + (oc \cdot oc) - r^2 = 0
$$

This is just a quadratic equation:

$$
At^2 + Bt + C_q = 0
$$

I use `C_q` for the quadratic constant so it does not collide with `C`, the sphere center. Sloppy symbol reuse is how bugs breed.

The coefficients are:

$$
A = D \cdot D
$$

$$
B = 2(oc \cdot D)
$$

$$
C_q = oc \cdot oc - r^2
$$

In C++:

```cpp
auto oc = r.origin() - center;

auto A = dot(r.direction(), r.direction());
auto B = 2.0 * dot(oc, r.direction());
auto C = dot(oc, oc) - radius * radius;
```

## Discriminant and ray hits

The discriminant tells whether the infinite line intersects the sphere:

$$
\Delta = B^2 - 4AC_q
$$

In C++:

```cpp
auto discriminant = B * B - 4 * A * C;
```

Interpretation:

- `discriminant < 0`: no real intersection
- `discriminant == 0`: one tangent intersection
- `discriminant > 0`: two intersections

But a ray is not an infinite line. A valid ray hit also needs at least one root with `t >= 0`.

```cpp
if (discriminant < 0) {
    return false;
}

auto sqrt_d = sqrt(discriminant);
auto t0 = (-B - sqrt_d) / (2.0 * A);
auto t1 = (-B + sqrt_d) / (2.0 * A);

bool hit = (t0 >= 0 || t1 >= 0);
```

Negative `t` values are behind the ray origin. They are mathematically valid line intersections, but not visible ray hits.

## Simplified coefficient form

A vector dotted with itself is the same as its squared length:

$$
D \cdot D = \lVert D \rVert^2
$$

$$
oc \cdot oc = \lVert oc \rVert^2
$$

So the code can be written as:

```cpp
auto oc = r.origin() - center;

auto A = r.direction().length_squared();
auto B = 2.0 * dot(oc, r.direction());
auto C = oc.length_squared() - radius * radius;
```

The book uses one more algebra trick to avoid carrying the full `B` term.

If:

$$
B = -2h
$$

then the quadratic formula simplifies:

$$
t = \frac{-B \pm \sqrt{B^2 - 4AC_q}}{2A}
$$

Substitute `B = -2h`:

$$
t = \frac{-(-2h) \pm \sqrt{(-2h)^2 - 4AC_q}}{2A}
$$

$$
t = \frac{2h \pm 2\sqrt{h^2 - AC_q}}{2A}
$$

$$
t = \frac{h \pm \sqrt{h^2 - AC_q}}{A}
$$

That is why the book can write:

$$
B = -2D \cdot (C - O)
$$

and therefore:

$$
h = \frac{B}{-2} = D \cdot (C - O)
$$

If the book writes the ray origin as `Q`, then `Q` is the same role as `O` here:

$$
h = D \cdot (C - Q)
$$

This works because the book uses:

$$
oc = C - O
$$

instead of:

$$
oc = O - C
$$

With the book's sign convention:

```cpp
auto oc = center - r.origin();

auto a = r.direction().length_squared();
auto h = dot(r.direction(), oc);
auto c = oc.length_squared() - radius * radius;

auto discriminant = h * h - a * c;
```

And the roots become:

```cpp
auto sqrt_d = sqrt(discriminant);
auto t0 = (h - sqrt_d) / a;
auto t1 = (h + sqrt_d) / a;
```

Same math. Cleaner coefficients. Different `oc` direction. Do not mix the two conventions unless you enjoy summoning bugs.

## Component version

The vector form compresses this longer component equation:

$$
(D_x^2 + D_y^2 + D_z^2)t^2
+ 2\left[(O_x - C_x)D_x + (O_y - C_y)D_y + (O_z - C_z)D_z\right]t
+ \left[(O_x - C_x)^2 + (O_y - C_y)^2 + (O_z - C_z)^2 - r^2\right]
= 0
$$

The dot product is just the clean way to express those repeated component patterns.

Use vectors. They are not hiding the math; they are expressing it at the correct level.

