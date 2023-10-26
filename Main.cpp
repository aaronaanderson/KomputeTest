#include <iostream>
#include <kompute/Kompute.hpp>

// copied from https://kompute.cc/overview/shaders-to-headers.html
static std::vector<uint32_t> compileSource (const std::string& source)
{
    std::ofstream fileOut("tmp_kp_shader.comp");
    fileOut << source;
    fileOut.close();
    if (system(std::string("glslangValidator -V tmp_kp_shader.comp -o tmp_kp_shader.comp.spv").c_str()))
        throw std::runtime_error("Error running glslangValidator command");
    std::ifstream fileStream("tmp_kp_shader.comp.spv", std::ios::binary);
    std::vector<char> buffer;
    buffer.insert(buffer.begin(), std::istreambuf_iterator<char>(fileStream), {});
    return {(uint32_t*)buffer.data(), (uint32_t*)(buffer.data() + buffer.size())};
}

void main()
{
    kp::Manager manager;

    std::shared_ptr<kp::TensorT<float>> tensorInA = manager.tensor ({2.0f, 4.0f, 6.0f});
    std::shared_ptr<kp::TensorT<float>> tensorInB = manager.tensor ({0.0f, 1.0f, 2.0f});
    std::shared_ptr<kp::TensorT<float>> tensorOut = manager.tensor ({0.0f, 0.0f, 0.0f});

    const std::vector<std::shared_ptr<kp::Tensor>> params = {tensorInA, tensorInB, tensorOut};

    std::string shader = (R"(
        #version 450
        
        // The execution structure
        layout (local_size_x = 1) in;
        
        // The buffers are provided via the tensors
        layout(binding = 0) buffer bufA { float a[]; };
        layout(binding = 1) buffer bufB { float b[]; };
        layout(binding = 2) buffer bufOut { float o[]; };
        
        void main() {
            uint index = gl_GlobalInvocationID.x;
            o[index] = a[index] * b[index];
        }
    )");

    auto algorithm = manager.algorithm (params, compileSource (shader));

    manager.sequence()->record<kp::OpTensorSyncDevice> (params)
                      ->record<kp::OpAlgoDispatch> (algorithm)
                      ->record<kp::OpTensorSyncLocal> (params)
                      ->eval();
    
    std::cout << "Output: ";
    for (const float& v : tensorOut->vector())
        std::cout << v << " ";
    
    //just to help find output
    for (int i = 0; i < 12; i ++) std::cout << "\n";
}