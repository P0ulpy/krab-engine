//
// Created by Flo on 26/11/2022.
//

#ifndef PATHFINDER_APPLICATIONLAYER_HPP
#define PATHFINDER_APPLICATIONLAYER_HPP

#include "../Time.hpp"

namespace KrabEngine
{
    class ApplicationLayer
    {
    public:
        explicit ApplicationLayer(const std::string_view& name = "Layer");
        virtual ~ApplicationLayer() = default;

        virtual void OnAttach() = 0;
        virtual void OnDetach() = 0;
        virtual void OnUpdate(Timestep ts) = 0;
        virtual void OnImGuiRender() { };

        [[nodiscard]] const std::string& GetName() const { return m_name; }
    protected:
        std::string m_name;
    };

} // Engine

#endif //PATHFINDER_APPLICATIONLAYER_HPP
