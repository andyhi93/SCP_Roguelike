#include "Tilemap.hpp"

#include "Util/Transform.hpp"
#include "Util/Logger.hpp"
#include "Util//Image.hpp"

Tilemap::Tilemap(){
    m_Transform.translation = { 0, 0 };
    this->SetDrawable(std::make_shared<Util::Image>("../../../Resources/Floor1.png"));
    m_Transform.scale = { 6,6 };
}
void Tilemap::Update() {

}
