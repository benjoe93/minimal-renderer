#include <map>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "vendor/imgui/imgui.h"
#include "vendor/stb_image/stb_image.h"

#include "LightDirectional.h"
#include "LightPoint.h"

#include "Renderer.h"
#include "Camera.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "Texture.h"
#include "Material.h"
#include "Mesh.h"
#include "Model.h"

#include "03_SceneTextures.h"


static float vertices[] = {
    // positions          // uvs        // colors           
     0.5f,  0.5f, 0.0f,   1.0f, 1.0f,   1.0f, 0.0f, 0.0f,   // top right
     0.5f, -0.5f, 0.0f,   1.0f, 0.0f,   0.0f, 1.0f, 0.0f,   // bottom right
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f,   0.0f, 0.0f, 1.0f,   // bottom left
    -0.5f,  0.5f, 0.0f,   0.0f, 1.0f,   1.0f, 1.0f, 0.0f    // top left 
};
static unsigned int indices[] = {  // note that we start from 0!
    3, 1, 0,  // first Triangle
    3, 2, 1   // second Triangle
};

namespace scene {
    SceneTextures::SceneTextures(Renderer& in_renderer)
        :Scene(in_renderer, "Texture Loading")
    {
        ////////////////////////////////////////////////////////////////////////////
        //                              load texture                              //
        ////////////////////////////////////////////////////////////////////////////

        int width, height, nrChannels;
        stbi_set_flip_vertically_on_load(true);
        // texture1
        glGenTextures(1, &tex1);
        glBindTexture(GL_TEXTURE_2D, tex1);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        unsigned char* data = stbi_load("resources/textures/container.jpg", &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            std::cout << "Faild to load texture" << std::endl;
        }
        stbi_image_free(data);
        // texture2
        glGenTextures(1, &tex2);
        glBindTexture(GL_TEXTURE_2D, tex2);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        data = stbi_load("resources/textures/awesomeface.png", &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            std::cout << "Failed to load texture" << std::endl;
        }
        stbi_image_free(data);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, tex1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, tex2);


        ////////////////////////////////////////////////////////////////////////////
        //                              shader setup                              //
        ////////////////////////////////////////////////////////////////////////////
        
        shader = std::make_unique<Shader>("resources/shaders/00_GettingStarted/texture_loading.vert", "resources/shaders/00_GettingStarted/texture_loading.frag");
        shader->Bind();
        shader->SetUniform("texture1", 0);
        shader->SetUniform("texture2", 1);

        ////////////////////////////////////////////////////////////////////////////
        //                            geometery setup                             //
        ////////////////////////////////////////////////////////////////////////////

        // vertex array object
        glGenVertexArrays(1, &VAO); // vertex array object
        glGenBuffers(1, &VBO);      // vertex buffer object
        glGenBuffers(1, &EBO);      // element buffer object
        // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
        glBindVertexArray(VAO);

        // bind and provide data to vbo
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // bind and provide data to ebo
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        // linking vertex attribs
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);                   // vertex position
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))); // uv coords
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float))); // vertex color
        glEnableVertexAttribArray(2);

        // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    void SceneTextures::OnUpdate(double delta_time)
    {}

    void SceneTextures::OnRender()
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw first triangle
        shader->Bind();
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }

    void SceneTextures::OnImGuiRender()
    {
    }
}