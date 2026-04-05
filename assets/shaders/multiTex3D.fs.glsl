#version 330

in vec2 vTexCoords;

out vec4 FragColor;

uniform sampler2D uEarthTexture;
uniform vec3 uColor;

void main() {

    vec3 texColor = texture(uEarthTexture, vTexCoords).rgb;
    FragColor = vec4(texColor * uColor, 1.0);
}