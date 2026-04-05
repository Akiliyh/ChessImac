#version 330

in vec2 vTexCoords;

out vec4 FragColor;

uniform sampler2D uEarthTexture;
uniform vec3 uColor;
uniform bool uUseTexture;

void main() {

    if(uUseTexture) {
        vec3 texColor = texture(uEarthTexture, vTexCoords).rgb;
        FragColor = vec4(texColor * uColor, 1.0);
    } else {
        FragColor = vec4(uColor, 1.0);
    }
}