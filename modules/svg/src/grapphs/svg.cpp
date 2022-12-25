#include <grapphs/svg.h>
#include <iomanip>

namespace gpp {
    SVGViewBox::SVGViewBox(
        float minX, float minY, float width, float height
    ) : minX(minX), minY(minY), width(width), height(height) {
    }

    SVGViewBox gpp::SVGViewBox::centralized(float width, float height) {
        return SVGViewBox(-width / 2, -height / 2, width, height);
    }

    SVGColor SVGColor::black() {
        return 0XFF000000;
    }

    std::ostream& operator<<(std::ostream& stream, const SVGColor& color) {
        return stream << '#' << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(color.r)
                      << std::setw(2) << static_cast<int>(color.g) << std::setw(2)
                      << static_cast<int>(color.b);
    }

    std::ostream& operator<<(std::ostream& os, const SVGAttributes& attributes) {
        os << "color: " << attributes.color << " size: " << attributes.size;
        return os;
    }
}