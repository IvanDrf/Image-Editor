#include "../Interface.hpp"

namespace Interface {
Position CalculateBrushSizePos(const Image& image) {
    return {kSmallMenuWidth + 4 * kButtonWidth + 0.9f * image.GetSpriteBound().size.x + kButtonWidth / 5, (kButtonHeight - kBrushBoxHeight) / 2};
}

Position CalculateBrushColorShapePos(const Position& position) {
    return {position.x + 1.35f * kBrushBoxWidth, position.y};
}

Position CalculatePalettePos(const Position& position) {
    return {position.x, position.y + kDefaultOutlineThickness};
}
}  // namespace Interface
