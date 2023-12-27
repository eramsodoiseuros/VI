//
// Created by sleim on 29/06/2023.
//

#include "ImageConverter.hpp"

// Converter: <arquivo_entrada> <formato> // Formato: jpg, pfm, exr
// Python Scripts: python programa <arquivo_entrada> <arquivo_saida>

void converter(const std::string& inputFile, const std::string& format) {
    std::string pythonScript;

    const std::string& outputFile_prefix = "../converted_images/";
    std::string outputFile = "";
    if (format == "jpg") {
        pythonScript = "../VI-RT/Image/Converters/ppm2jpg.py";
        outputFile = outputFile_prefix + "JPG.jpg";
    } else if (format == "pfm") {
        pythonScript = "../VI-RT/Image/Converters/ppm2pfm.py";
        outputFile = outputFile_prefix + "PFM.pfm";
    } else if (format == "exr") {
        pythonScript = "../VI-RT/Image/Converters/ppm2openexr.py";
        outputFile = outputFile_prefix + "EXR.exr";
    } else {
        std::cerr << "Formato inválido." << std::endl;
        return;
    }


    std::string command = "python " + pythonScript + " " + inputFile + " " + outputFile;

    int result = std::system(command.c_str());

    if (result == 0) {
        std::cout << "Arquivo Python executado com sucesso!" << std::endl;
    } else {
        std::cerr << "Erro ao executar o arquivo Python." << std::endl;
    }
}