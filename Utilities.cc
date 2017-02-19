#include <fstream>
#include <iterator>

#include "Utilities.hh"

using namespace Animate;

/**
 * Read the given file into a raw byte vector.
 *
 * @param filename The filename of the file to read.
 *
 * @return A byte array.
 */
std::vector<GLbyte> Utilities::read_file(std::string filename) {
    std::ifstream file(filename.c_str(), std::ios::binary);
    file.unsetf(std::ios::skipws);

    file.seekg(0, std::ios::end);
    std::vector<GLbyte> bytes;
    bytes.reserve(file.tellg());
    file.seekg(0, std::ios::beg);

    bytes.insert(
        bytes.begin(),
        std::istream_iterator<GLbyte>(file),
        std::istream_iterator<GLbyte>()
    );

    return bytes;
}
