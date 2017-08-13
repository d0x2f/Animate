"""Converts resource files into C++ code to be embedded in the compiled binary."""

import json

def generate_code():
    """Create Resource.cc containing all the binary data from files listed in reouces.json"""
    with open("resources.json", 'r') as file:
        resource_list = json.load(file)

    with open('src/Resources.cc', 'w') as output:
        print('#include "Resources.hh"\n', file=output)
        print('using namespace Animate;\n', file=output)
        print('std::map<std::string,std::vector<uint8_t> > Resources::resources;\n', file=output)
        print('void Resources::initialise(){', file=output)
        i = 0
        for path in resource_list:
            print('std::vector<uint8_t> data_%d = {' % i, file=output)
            with open(path, "rb") as resource:
                data = resource.read()
                byte_data = bytearray(data)
                print(', '.join("0x%02x" % x for x in byte_data), file=output)
            print('\n};', file=output)
            print(
                'Resources::resources.insert('
                + 'std::pair<std::string,std::vector<uint8_t> >("%s", data_%d));'
                % (path, i),
                file=output
            )
            i += 1
        print('}', file=output)

generate_code()
