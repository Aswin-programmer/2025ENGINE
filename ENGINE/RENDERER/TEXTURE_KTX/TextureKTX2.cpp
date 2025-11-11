#include "TextureKTX2.h"

TextureKTX2::TextureKTX2(std::string filepath, struct TextureType::NORMAL)
	:
	textureID{ 0 },
	target{ GL_TEXTURE_2D }
{
	//LoadTX2Texture(ConvertPNG2KTG2(filepath).c_str());
	LoadTX2Texture(filepath.c_str());
}

TextureKTX2::TextureKTX2(std::string filepath, struct TextureType::CUBEMAP)
	:
	textureID{ 0 },
	target{ GL_TEXTURE_2D }
{
	LoadKTX2CubeMap(filepath.c_str());
}

TextureKTX2::TextureKTX2(std::string filepath, GLenum target)
	:
	textureID{0},
	target{target}
{
	//LoadTX2Texture(ConvertPNG2KTG2(filepath).c_str());
	//LoadTX2Texture(filepath.c_str());
	//LoadKTX2CubeMap(filepath.c_str());
}

TextureKTX2::~TextureKTX2()
{
	glBindTexture(target, 0);
	glDeleteTextures(1, &textureID);
}

void TextureKTX2::Bind(int texture_unit)
{
	glBindTextureUnit(texture_unit, textureID);
}

void TextureKTX2::UnBind()
{
	glBindTextureUnit(0, textureID);
}

bool TextureKTX2::LoadTX2Texture(const char* filePath)
{
	// 1. Check for valid OpenGL context
	if (!glfwGetCurrentContext()) {
		std::cout << "No OpenGL context available for texture loading" << std::endl;
		return false;
	}

	// 2. Check file exists
	if (!std::filesystem::exists(filePath)) {
		std::cout << "KTX2 file does not exist: " << filePath << std::endl;
		return false;
	}

	// 3. Load texture from file
	ktxTexture2* texture;
	KTX_error_code result = ktxTexture2_CreateFromNamedFile(
		filePath,
		KTX_TEXTURE_CREATE_LOAD_IMAGE_DATA_BIT,
		&texture
	);

	if (result != KTX_SUCCESS) {
		std::cout << "Failed to load KTX2 Texture: " << ktxErrorString(result) << std::endl;
		return false;
	}

	// 4. Transcode if needed
	if (ktxTexture2_NeedsTranscoding(texture)) {
		KTX_error_code err = ktxTexture2_TranscodeBasis(
			texture,
			KTX_TTF_BC3_RGBA, // BC3 (DXT5) is widely supported
			0
		);
		if (err != KTX_SUCCESS) {
			std::cout << "Failed to transcode: " << ktxErrorString(err) << std::endl;
			ktxTexture_Destroy(reinterpret_cast<ktxTexture*>(texture));
			return false;
		}
	}

	// 5. Upload texture to GPU (will generate texture object)
	result = ktxTexture_GLUpload(reinterpret_cast<ktxTexture*>(texture), &textureID, &target, nullptr);
	if (result != KTX_SUCCESS) {
		std::cout << "Failed to upload texture to GPU: " << ktxErrorString(result) << std::endl;
		ktxTexture_Destroy(reinterpret_cast<ktxTexture*>(texture));
		return false;
	}

	// 6. Destroy CPU-side KTX texture
	ktxTexture_Destroy(reinterpret_cast<ktxTexture*>(texture));

	// 7. Set texture parameters with DSA
	glTextureParameteri(textureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTextureParameteri(textureID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTextureParameteri(textureID, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(textureID, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// 8. Optional: Check if texture is compressed (DSA)
	GLint isCompressed = 0;
	glGetTextureLevelParameteriv(textureID, 0, GL_TEXTURE_COMPRESSED, &isCompressed);
	if (isCompressed == GL_TRUE) {
		std::cout << "The texture with ID: {" << textureID << "} is compressed! Saves memory." << std::endl;
	}
	else {
		std::cout << "The texture with ID: {" << textureID << "} is not compressed!" << std::endl;
	}

	return true;
}

bool TextureKTX2::LoadTX2Texture2D(const char* filePath)
{
	// Verify OpenGL context is available before proceeding
	if (!glfwGetCurrentContext())
	{
		std::cout << "No OpenGL context available for texture loading" << std::endl;
		return false;
	}

	// Check if file exists
	if (!std::filesystem::exists(filePath))
	{
		std::cout << "KTX2 file does not exist: " << filePath << std::endl;
		return false;
	}

	ktxTexture2* texture;
	KTX_error_code result = ktxTexture2_CreateFromNamedFile(
		filePath,
		KTX_TEXTURE_CREATE_LOAD_IMAGE_DATA_BIT,
		&texture
	);
	if (result != KTX_SUCCESS)
	{
		std::cout << "Failed to load KTX2 Texture: " << ktxErrorString(result) << std::endl;
		return false;
	}

	if (ktxTexture2_NeedsTranscoding(texture)) {
		KTX_error_code err = ktxTexture2_TranscodeBasis(
			texture, KTX_TTF_BC3_RGBA, 0);
		if (err != KTX_SUCCESS) {
			std::cout << "Failed to transcode: " << ktxErrorString(err) << std::endl;
			return false;
		}
	}

	// Create an OpenGL texture object
	glGenTextures(1, &textureID);
	glBindTexture(target, textureID);

	result = ktxTexture_GLUpload(reinterpret_cast<ktxTexture*>(texture), &textureID, &target, nullptr);
	if (result != KTX_SUCCESS)
	{
		std::cout << "Failed to upload texture to the GPU: " << ktxErrorString(result) << std::endl;
		ktxTexture_Destroy(reinterpret_cast<ktxTexture*>(texture));
		return false;
	}
	ktxTexture_Destroy(reinterpret_cast<ktxTexture*>(texture));

	glBindTexture(target, textureID);
	glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_REPEAT);

	GLint isCompressed;
	glGetTexLevelParameteriv(target, 0, GL_TEXTURE_COMPRESSED, &isCompressed);
	if (isCompressed == GL_TRUE) {
		// Texture is compressed
		std::cout << "The texture with ID: " << "{" << textureID << "}" << " is compressed!. SAVES MEMORY!." << std::endl;
	}
	else
	{
		std::cout << "The texture with ID: " << "{" << textureID << "}" << " is not compressed!" << std::endl;
	}

	glBindTexture(target, 0);

	return true;
}

bool TextureKTX2::LoadKTX2CubeMap(const char* filePath)
{
	if (!glfwGetCurrentContext()) {
		std::cout << "No OpenGL context available for texture loading" << std::endl;
		return false;
	}
	if (!std::filesystem::exists(filePath)) {
		std::cout << "KTX2 file does not exist: " << filePath << std::endl;
		return false;
	}

	ktxTexture2* texture = nullptr;
	KTX_error_code result = ktxTexture2_CreateFromNamedFile(
		filePath,
		KTX_TEXTURE_CREATE_LOAD_IMAGE_DATA_BIT,
		&texture
	);
	if (result != KTX_SUCCESS) {
		std::cout << "Failed to load KTX2 Texture: " << ktxErrorString(result) << std::endl;
		return false;
	}

	// If transcode needed (Basis-compressed inside), transcode to a desktop-friendly format
	if (ktxTexture2_NeedsTranscoding(texture)) {
		KTX_error_code err = ktxTexture2_TranscodeBasis(texture, KTX_TTF_BC3_RGBA, 0);
		if (err != KTX_SUCCESS) {
			std::cout << "Failed to transcode: " << ktxErrorString(err) << std::endl;
			ktxTexture_Destroy(reinterpret_cast<ktxTexture*>(texture));
			return false;
		}
	}

	// Force our target to be CUBE_MAP
	target = GL_TEXTURE_CUBE_MAP;

	// Upload. ktxTexture_GLUpload will create/open the GL texture if we pass textureID/address.
	result = ktxTexture_GLUpload(reinterpret_cast<ktxTexture*>(texture), &textureID, &target, nullptr);
	if (result != KTX_SUCCESS) {
		std::cout << "Failed to upload KTX2 cubemap to GPU: " << ktxErrorString(result) << std::endl;
		ktxTexture_Destroy(reinterpret_cast<ktxTexture*>(texture));
		return false;
	}

	// Done with CPU-side ktx texture
	ktxTexture_Destroy(reinterpret_cast<ktxTexture*>(texture));

	glTextureParameteri(textureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTextureParameteri(textureID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTextureParameteri(textureID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTextureParameteri(textureID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTextureParameteri(textureID, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	// Enable seamless cubemap sampling
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	// Optional debug: is compressed?
	GLint isCompressed = 0;
	glGetTextureLevelParameteriv(textureID, 0, GL_TEXTURE_COMPRESSED, &isCompressed);
	std::cout << "Cubemap texture " << textureID << " compressed? " << (isCompressed ? "yes" : "no") << std::endl;

	return true;
}

bool TextureKTX2::ConvertPNGtoKTX2Texture(const std::string& pngSourcePath, const std::string& ktxOutputPath)
{
	// Resolve full paths
	std::filesystem::path inputPath = std::filesystem::absolute(pngSourcePath);
	std::filesystem::path outputPath = std::filesystem::absolute(ktxOutputPath);

	//Validate Inputs:
	if (!std::filesystem::exists(inputPath))
	{
		std::cout << "The input file doesn't exist: " << inputPath << std::endl;
		return false;
	}

	// Resolve toktx path safely
	const char* toktxEnv = std::getenv("TOKTX_PATH");
	std::string toktxPath = toktxEnv ? std::string(toktxEnv) : "toktx";

	// Prepare output directory
	std::filesystem::path outputDir = std::filesystem::path(outputPath).parent_path();
	if (!outputDir.empty() && !std::filesystem::exists(outputDir))
	{
		std::filesystem::create_directories(outputDir);
	}

	// Build a safe command
	std::string cmd = toktxPath;
	cmd += " --bcmp";          // Basis Universal compression << THIS LINE IS CAUSING ERRORS >>
	cmd += " --genmipmap";     // Generate mipmaps
	cmd += " --t2";            // Use KTX2 format
	cmd += " \"" + outputPath.string() + "\"";
	cmd += " \"" + inputPath.string() + "\"";

	//Execute with better error handling
	std::cout << "Executing: " << cmd << std::endl;
	int result = std::system(cmd.c_str());

	// Check results
	if (result != 0)
	{
		std::cout << "Conversion failed with code: " << result << std::endl;
		if (std::filesystem::exists(outputPath))
		{
			std::filesystem::remove(outputPath);
		}
		return false;
	}

	// Verify output
	if (!std::filesystem::exists(outputPath) || std::filesystem::file_size(outputPath) == 0)
	{
		std::cout << "The output file is not created: " << outputPath << std::endl;
		return false;
	}

	return true;
}

std::string TextureKTX2::ConvertPNG2KTG2(const std::string& pngPath)
{
	std::string ktxPath = pngPath;

	// Replace "png" to "ktx2"
	size_t folderPos = ktxPath.find("\\PNG\\");
	if (folderPos != std::string::npos)
	{
		ktxPath.replace(folderPos, 5, "\\KTX\\");
	}

	// Replace ".png" with ".ktx2"
	size_t extPos = ktxPath.rfind(".png");
	if (extPos != std::string::npos)
	{
		ktxPath.replace(extPos, 4, ".ktx2");
	}

	return ktxPath;

}
