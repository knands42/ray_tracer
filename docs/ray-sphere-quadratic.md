# Ray-Sphere Quadratic Intuition

A ray gives points through space:

```text
P(t) = O + tD
```

Where:

```text
O = ray origin
D = ray direction
t = distance/scale along the ray
```

A sphere is every point whose distance from its center is the radius:

```text
dot(P - C, P - C) = r²
```

Where:

```text
C = sphere center
r = sphere radius
```

If the sphere is centered at the origin, `C = (0,0,0)`, this simplifies to:

```text
dot(P, P) = r²
```

Because `dot(P, P)` means the squared distance from the origin:

```text
P.x² + P.y² + P.z²
```

## Substitute the ray into the sphere

Use the ray point `P(t)` inside the sphere equation:

```text
dot(O + tD - C, O + tD - C) = r²
```

Define:

```text
oc = O - C
```

Then:

```text
dot(oc + tD, oc + tD) = r²
```

Expand into a quadratic:

```text
dot(D,D)t² + 2dot(oc,D)t + dot(oc,oc) - r² = 0
```

Which matches:

```text
A*t² + B*t + C = 0
```

So the coefficients are:

```cpp
auto oc = r.origin() - center;

auto A = dot(r.direction(), r.direction());
auto B = 2.0 * dot(oc, r.direction());
auto C = dot(oc, oc) - radius * radius;
```

The discriminant tells whether the infinite line intersects the sphere:

```cpp
auto discriminant = B * B - 4 * A * C;
```

For a real ray hit, also check that at least one root has `t >= 0`:

```cpp
auto sqrt_d = sqrt(discriminant);
auto t0 = (-B - sqrt_d) / (2.0 * A);
auto t1 = (-B + sqrt_d) / (2.0 * A);

bool hit = discriminant >= 0 && (t0 >= 0 || t1 >= 0);
```

## Component version

The vector form compresses this longer component equation:

```text
(Dx² + Dy² + Dz²)t²
+ 2[(Ox-Cx)Dx + (Oy-Cy)Dy + (Oz-Cz)Dz]t
+ [(Ox-Cx)² + (Oy-Cy)² + (Oz-Cz)² - r²]
= 0
```

The dot product is just the clean way to express those repeated component patterns.

Use vectors. They are not hiding the math — they are expressing it at the correct level.
