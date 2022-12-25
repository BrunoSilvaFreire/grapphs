#include <grapphs/svg.h>

namespace gpp {
    SVGViewBox::SVGViewBox(
        float minX, float minY, float width, float height
    ) : minX(minX), minY(minY), width(width), height(height) {
    }

    SVGViewBox gpp::SVGViewBox::centralized(float width, float height) {
        return SVGViewBox(-width / 2, -height / 2, width, height);
    }
}