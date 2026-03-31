#version 330

in vec2 vTexCoords;

out vec3 fFragColor;
uniform sampler2D uEarthTexture;
uniform sampler2D uCloudTexture;

void main() {
    vec3 mixedTextureColors = (texture(uEarthTexture, vTexCoords)).rgb + (texture(uCloudTexture, vTexCoords)).rgb;
    fFragColor = mixedTextureColors;
}
