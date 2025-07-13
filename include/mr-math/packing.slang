//
// Octahedral normal packing in Slang
//

module packing;

static const float max_value16 = 65535.0;
static const float max_value12 = 4095.0;
static const float max_value8 = 255.0;

float2 sign_non_zero(float2 v) {
    return float2(v.x >= 0.0 ? 1.0 : -1.0,
                  v.y >= 0.0 ? 1.0 : -1.0);
}

float2 oct_wrap(float2 v) {
    float2 abs_v = abs(v);
    return (float2(1.0) - abs_v.yx) * sign_non_zero(v);
}

float2 pack_oct_impl(float3 norm) {
    float denom = abs(norm.x) + abs(norm.y) + abs(norm.z);
    float2 oct = float2(norm.x, norm.y) / denom;
    return (norm.z < 0.0) ? oct_wrap(oct) : oct;
}

float3 unpack_oct_impl(float2 packed_norm) {
    float3 v = float3(packed_norm, 1.0 - abs(packed_norm.x) - abs(packed_norm.y));
    if (v.z < 0.0) {
        v.xy = oct_wrap(packed_norm);
    }
    return normalize(v);
}

// 32-bit packing (16 bits per component)
uint pack_oct32(float3 norm) {
    float2 oct = pack_oct_impl(norm);
    float2 quantized = clamp(oct * 0.5 + 0.5, 0.0, 1.0);
    uint2 packed = uint2(quantized * max_value16);
    return (packed.x << 16) | packed.y;
}

float3 unpack_oct32(uint packed_norm) {
    uint x = (packed_norm >> 16) & 0xFFFF;
    uint y = packed_norm & 0xFFFF;
    float2 dequantized = float2(x, y) / max_value16;
    float2 oct = dequantized * 2.0 - 1.0;
    return unpack_oct_impl(oct);
}

// 24-bit packing (12 bits per component)
uint pack_oct24(float3 norm) {
    float2 oct = pack_oct_impl(norm);
    float2 quantized = clamp(oct * 0.5 + 0.5, 0.0, 1.0);
    uint2 packed = uint2(quantized * max_value12);
    return (packed.x << 12) | packed.y; // Uses lower 24 bits
}

float3 unpack_oct24(uint packed_norm) {
    uint x = (packed_norm >> 12) & 0xFFF;
    uint y = packed_norm & 0xFFF;
    float2 dequantized = float2(x, y) / max_value12;
    float2 oct = dequantized * 2.0 - 1.0;
    return unpack_oct_impl(oct);
}

// 16-bit packing (8 bits per component)
uint16_t pack_oct16(float3 norm) {
    float2 oct = pack_oct_impl(norm);
    float2 quantized = clamp(oct * 0.5 + 0.5, 0.0, 1.0);
    uint2 packed = uint2(quantized * max_value8);
    return (packed.x << 8) | packed.y;
}

float3 unpack_oct16(uint16_t packed_norm) {
    uint x = (packed_norm >> 8) & 0xFF;
    uint y = packed_norm & 0xFF;
    float2 dequantized = float2(x, y) / max_value8;
    float2 oct = dequantized * 2.0 - 1.0;
    return unpack_oct_impl(oct);
}
