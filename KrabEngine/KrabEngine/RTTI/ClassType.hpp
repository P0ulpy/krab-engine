//
// Created by Flo on 14/12/2022.
//

#ifndef PATHFINDER_RTTI_HPP
#define PATHFINDER_RTTI_HPP

#include <string>
#include <utility>

#define DECLARE_CLASS_TYPE(ClassName, ParentType) \
    static RTTI::ClassType* getClassType() { static RTTI::ClassType classType{#ClassName, ParentType::getClassType()}; return &classType; } \
    virtual RTTI::ClassType* getType() const { return getClassType(); }

namespace RTTI
{
    class ClassType
    {
    public:
        using ClassTypeName = std::string;

        inline ClassType(ClassTypeName className, ClassType* parentClassType)
            : m_typeName(std::move(className))
            , m_parentClassType(parentClassType)
        {}

        [[nodiscard]] inline const ClassTypeName& getTypeName() const { return m_typeName; }
        [[nodiscard]] inline const ClassType* getParentType() const { return m_parentClassType; }
        [[nodiscard]] inline bool isInstanceOrChildOf(ClassType* Parent) const;

    private:
        ClassTypeName m_typeName;
        ClassType* m_parentClassType;
    };

    class IClassType
    {
    public:
        virtual ~IClassType() = default;
        virtual ClassType* getType() const = 0;
    };

    class NoClassTypeAncestor
    {
    public:
        static ClassType* getClassType() { return nullptr; }
    };

    bool ClassType::isInstanceOrChildOf(ClassType *Parent) const {
        if (Parent->getTypeName() == m_typeName)
            return true;
        const ClassType* CurrentClass = m_parentClassType;
        while (CurrentClass != NoClassTypeAncestor::getClassType())
        {
            if (CurrentClass->getTypeName() == Parent->getTypeName())
                return true;
            CurrentClass = m_parentClassType->getParentType();
        }
        return false;
    }
}

#endif //PATHFINDER_RTTI_HPP
