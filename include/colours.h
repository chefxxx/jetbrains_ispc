//
// Created by Mateusz Mikiciuk on 25/10/2025.
//

#ifndef COLOURS_H
#define COLOURS_H

struct RGB {
    RGB(const int R, const int G, const int B) : r(R), g(G), b(B) {}
    uint8_t r, g, b;
};

/**
 * C++'s implementation of
 * https://stackoverflow.com/questions/17242144/how-to-convert-hsb-hsv-color-to-rgb-accurately.
 */
inline RGB HSVtoRGB(float H, const float S, const float V) {
    H = std::fmod(H, 1.0f);
    if (H < 0)
        H += 1.0f;
    const float i = std::floor(6.0 * H);
    const float f = H * 6 - i;
    const float p = V * (1.0f - S);
    const float q = V * (1.0f - S * f);
    const float t = V * (1.0f - S * (1.0f - f));
    float r = 0, g = 0, b = 0;
    switch (static_cast<int>(i) % 6) {
    case 0:
        r = V, g = t, b = p;
        break;
    case 1:
        r = q, g = V, b = p;
        break;
    case 2:
        r = p, g = V, b = t;
        break;
    case 3:
        r = p, g = q, b = V;
        break;
    case 4:
        r = t, g = p, b = V;
        break;
    case 5:
        r = V, g = p, b = q;
        break;
    default:
        std::cerr << "Invalid color index\n";
    }
    return RGB{static_cast<int>(std::round(r * 255.0f)), static_cast<int>(std::round(g * 255.0f)),
               static_cast<int>(std::round(b * 255.0f))};
}

#endif // COLOURS_H
