#include <experimental/filesystem>

#include "ResourceManager.hh"
#include "Utilities.hh"

using namespace Animate;

ResourceManager::ResourceManager()
{
}

ResourceManager::~ResourceManager()
{
}

void ResourceManager::load_from_directory(std::string directory)
{
    for (std::experimental::filesystem::recursive_directory_iterator i(directory), end; i != end; ++i) {
        if (!std::experimental::filesystem::is_directory(i->path())) {
            //Remove './resources/' from the key
            std::string key = i->path().relative_path().string().erase(0, 11);

            //Read in raw data
            std::vector<GLbyte> data = Utilities::read_file(i->path());

            //Add to data store
            this->data_store.insert(std::pair< std::string, std::vector<GLbyte> >(key, data));
        }
    }
}

std::vector<GLbyte> ResourceManager::get_resource(std::string name)
{
    auto search = this->data_store.find(name);
    if (search != this->data_store.end()) {
        return search->second;
    }
}

std::string ResourceManager::get_resource_as_string(std::string name)
{
    std::vector<GLbyte> data = this->get_resource(name);
    return std::string(data.begin(), data.end());
}