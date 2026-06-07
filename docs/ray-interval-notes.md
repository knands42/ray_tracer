# Ray Interval Notes

## What the Interval Actually Does

The ray-sphere intersection solves a quadratic equation. That equation finds where the ray hits the sphere along an **infinite line**. But a ray is not infinite—it starts at a point and goes forward. The interval `[t_min, t_max]` is what turns the math from "infinite line" into "ray."

`t` is the parameter in `P(t) = origin + t * direction`. It scales the direction vector. Think of it as "how far along the ray."

The interval check `surrounds(t)` simply asks: is this `t` value between `t_min` and `t_max`? If not, the intersection is discarded—even if the math found a valid hit.

## Choosing `t_min` and `t_max`

**For primary rays** (camera shooting into the scene):
- `t_min = 0.001` (small epsilon)
- `t_max = infinity`

The epsilon exists because of floating-point imprecision. Without it, the ray might detect the surface it just left as a hit (self-intersection → shadow acne). The epsilon pushes the valid range just past the origin point.

**For shadow rays** (checking if a point is lit):
- `t_min = 0.001`
- `t_max = distance to light source`

Anything beyond the light cannot cast a shadow on the point, so there is no reason to look past it. This also speeds up the test.

**For scattered/bounced rays** (diffuse reflection):
- `t_min = 0.001`
- `t_max = infinity`

The scattered ray starts at a hit surface and bounces outward. It needs to check everything in front of it.

## How Do You Know `t` Will Be Positive?

You do not need to worry about it. The direction vector defines "forward." Any object the ray can reach is at `t > 0` by construction.

Example: camera at `(0,0,0)`, looking into `-Z`, direction `(0,0,-1)`. An object at `(0,0,-5)`:

```
(0,0,-5) = (0,0,0) + t * (0,0,-1)
t = 5  (positive)
```

The Z coordinate is negative but `t` is positive because the direction vector also points negative. The signs cancel. `t` is positive whenever the target lies in the same direction the ray points.

Negative `t` means the object is behind the ray origin. The interval `[0.001, ...]` automatically discards these.

## What Happens When the Camera Moves

Nothing breaks. This is the key insight: **the interval is always relative to the ray's own origin, not to any fixed world position.**

When the camera moves to a new position, each primary ray gets a new origin. The direction vectors update to point through each pixel from the new camera position. The interval `[0.001, infinity]` still works because:

- `t = 0` is still "at the ray origin" (which is now the new camera position)
- `t > 0` is still "in front of the ray"
- The epsilon still prevents self-intersection

The same applies to scattered rays. Each bounce creates a new ray with a new origin (the hit point). The interval `[0.001, infinity]` is valid for every single one, regardless of where in the scene the bounce happened.

## When the Interval Approach Does Not Work

The interval assumes rays have a clear origin and forward direction. Some cases need different logic:

- **"Is anything within radius R of this point?"** — Not a ray query. Test distance from the point to each sphere directly. No `t` parameter involved.

- **Omnidirectional light at a point** — If you want to know all spheres touching a point from any direction, rays are the wrong tool. Use distance checks.

- **Volumetric effects (fog)** — You care about all `t` values along the ray, not just the first hit. The interval still bounds the search, but the processing inside it is different.

## Common Mistake: Mixing Up `t` and World Coordinates

`t` is not a coordinate. It is a scalar that scales the direction vector. A sphere at `z = -100` can have `t = 2` if the direction vector is `(0, 0, -50)`. A sphere at `z = -2` can have `t = 100` if the direction vector is `(0, 0, -0.02)`.

Do not confuse the value of `t` with distance in world space. They are only equal when the direction vector is a unit vector (length 1). If the direction is not normalized, `t` is a scaled distance, not an actual distance.

## Summary

| Ray Type | t_min | t_max | Why |
|---|---|---|---|
| Primary (camera) | 0.001 | infinity | Everything in front of camera |
| Shadow | 0.001 | dist to light | Only up to the light matters |
| Scattered (bounce) | 0.001 | infinity | Everything in front of hit surface |

- `t > 0` is guaranteed for anything in front of the ray
- The interval moves with the ray origin—camera movement does not break it
- `t` is not world distance unless direction is normalized
