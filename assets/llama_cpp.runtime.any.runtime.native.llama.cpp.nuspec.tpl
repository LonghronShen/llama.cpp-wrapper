<?xml version="1.0" encoding="utf-8"?>
<package xmlns="http://schemas.microsoft.com/packaging/2013/01/nuspec.xsd">
  <metadata minClientVersion="2.12">
    <id>llama_cpp.runtime.any.runtime.native.llama.cpp</id>
    <version>${RELEASE_VERSION}</version>
    <title>llama_cpp.runtime.any.runtime.native.llama.cpp</title>
    <authors>longhronshen</authors>
    <owners>longhronshen</owners>
    <readme>docs\README.md</readme>
    <requireLicenseAcceptance>false</requireLicenseAcceptance>
    <license type="expression">MIT</license>
    <projectUrl>https://github.com/LonghronShen/llma.cpp-wrapper</projectUrl>
    <description>Native pacakge for llama_cpp. Compatible with LllamaSharp.Backend.</description>
    <serviceable>true</serviceable>
    <dependencies>
      <dependency id="llama_cpp.runtime.ubuntu.20.04-arm64.runtime.native.llama.cpp.GNU"
        version="${RELEASE_VERSION}" />
      <dependency id="llama_cpp.runtime.ubuntu.20.04-x64.runtime.native.llama.cpp.GNU"
        version="${RELEASE_VERSION}" />
      <dependency id="llama_cpp.runtime.ubuntu.18.04-x86.runtime.native.llama.cpp.GNU"
        version="${RELEASE_VERSION}" />
      <dependency id="llama_cpp.runtime.osx.12.6.5-x64.runtime.native.llama.cpp.AppleClang"
        version="${RELEASE_VERSION}" />
      <dependency id="llama_cpp.runtime.win-arm64.runtime.native.llama.cpp.MSVC"
        version="${RELEASE_VERSION}" />
      <dependency id="llama_cpp.runtime.win-x64.runtime.native.llama.cpp.MSVC"
        version="${RELEASE_VERSION}" />
      <dependency id="llama_cpp.runtime.win-x86.runtime.native.llama.cpp.MSVC"
        version="${RELEASE_VERSION}" />
    </dependencies>
  </metadata>
  <files>
    <file src="README.md" target="docs\" />
  </files>
</package>