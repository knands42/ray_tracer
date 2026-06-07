# Linear Space, Gamma Space, and Gamma Correction

## Linear Space

Ray tracing computes lighting in **linear space**. When two light sources illuminate a surface, their contributions add directly: 0.5 + 0.5 = 1.0. This matches how light behaves in the physical world—energy is additive.

All intermediate calculations (ray bounces, color accumulation, shading) should happen in linear space to produce physically correct results.

## Gamma Space

**Gamma space** is a non-linear encoding of color values. The relationship is:

$$V_{display} = V_{linear}^{1/\gamma}$$

For gamma 2 (a common approximation):

$$V_{display} = V_{linear}^{1/2} = \sqrt{V_{linear}}$$

This is exactly what the code does:

```cpp
inline double linear_to_gamma(double linear_component)
{
    if (linear_component > 0)
    {
        return std::sqrt(linear_component);
    }
    return 0;
}
```

The square root compresses bright values and expands dark values. A linear value of 0.25 becomes 0.5 in gamma space; a linear value of 0.81 becomes 0.9.

## Why Image Viewers Expect Gamma Space

Display hardware (monitors, phones) have a non-linear response: the voltage sent to a pixel does not produce a linearly proportional brightness. Instead, displays approximately follow:

$$V_{perceived} = V_{input}^{\gamma}$$

where γ ≈ 2.2 for most modern displays.

If you send a linear value of 0.5 directly to the display, the monitor applies its own power curve and the pixel appears at brightness 0.5^2.2 ≈ 0.22—much darker than intended.

By pre-applying gamma correction (encoding to gamma space), the two curves cancel out:

$$V_{final} = (V_{linear}^{1/\gamma})^{\gamma} = V_{linear}$$

The viewer sees the correct brightness.

## The Pipeline

```
Scene (physical light)
  → Ray tracing (linear space, additive math)
  → linear_to_gamma() (encode to gamma space)
  → Write to image file (PNG, JPEG, etc.)
  → Display applies its power curve
  → Viewer sees correct brightness
```

## Why Not Skip It?

Without gamma correction, images appear **too dark**, especially in mid-tones and shadows. Dark regions lose detail because the display's power curve crushes them further. Highlights look correct by coincidence (1.0 maps to 1.0 regardless), but everything else shifts downward.

## Why the `> 0` Guard

The square root of a negative number is undefined in real arithmetic. In practice, linear color components should never be negative, but floating-point rounding during accumulation can produce tiny negative values (e.g., -1e-15). The guard clamps these to zero before the sqrt call.

## Appendix: Why Displays Have a Non-Linear Response

### It Started with CRT Physics

The V^γ curve is not a designed behavior—it emerged from how CRT (cathode ray tube) monitors worked. An electron gun fires a beam at phosphor-coated glass. The beam current depends non-linearly on the control grid voltage (thermionic emission follows the Richardson-Dushman equation). The phosphor's light output is also non-linear with respect to beam energy. Chaining these two non-linearities produces an approximate power law:

$$L = V^{\gamma}$$

where γ ≈ 2.2–2.5 depending on the phosphor. There is no clean first-principles derivation of the exact exponent—it is an empirical observation of the combined CRT system.

### Why the Curve Survived

Flat-panel displays (LCD, OLED) replaced CRTs and could have implemented a linear response. Two reasons they did not:

1. **Backward compatibility.** All existing images and video were already gamma-encoded. Sending those values to a linear display would produce wrong colors.

2. **Perceptual efficiency.** Human vision is more sensitive to differences in dark tones than bright tones. A gamma curve allocates more code values to dark regions where we can see small differences, and fewer to bright regions where we cannot. An 8-bit gamma-encoded image stores more *perceptually useful* information than an 8-bit linear image.

### The Modern Chain

Modern displays still expect gamma-encoded input. The sRGB standard defines a curve approximately equal to γ = 2.2 (piecewise: linear near black, power law above). The full chain:

```
Linear light values
  → Encode with 1/2.2 (your sqrt for gamma 2)
  → Store/transmit gamma-encoded values
  → Display applies ~2.2 power curve
  → Viewer sees approximately linear brightness
```

HDR workflows (scRGB, linear float EXR) skip this and store linear values, letting the display handle conversion. But for standard 8-bit SDR content, gamma encoding remains the standard because it makes better use of limited bit depth.
