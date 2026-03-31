#version 330

in vec2 vTexCoords;

out vec3 fFragColor;
uniform sampler2D uTexture;

void main() {
    texture(uTexture, vTexCoords);
    fFragColor = (texture(uTexture, vTexCoords)).rgb;
}
