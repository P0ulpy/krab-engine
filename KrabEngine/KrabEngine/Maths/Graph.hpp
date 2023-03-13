#pragma once

#include <array>
#include <vector>
#include <functional>
#include <stdexcept>
#include <iostream>
#include <cmath>
#include <algorithm>

namespace Maths::Graph {
    template <typename NT>
    class BinaryTree;
    template <typename NT>
    class KdTree;

    template <typename CT>
    class GraphNode
    {
    public:
        using ContentT = CT;

        bool operator==(const GraphNode& Other)
        {
            return m_Content == Other.m_Content;
        }

        virtual ~GraphNode() = default;

        GraphNode(const GraphNode& Node) = default;
        GraphNode& operator=(const GraphNode& Node) = default;

        ContentT& GetContent()
        {
            return m_Content;
        }

        ContentT& GetContent() const
        {
            return m_Content;
        }
    protected:
        explicit GraphNode(ContentT Content) : m_Content(Content) {};

        mutable ContentT m_Content;
    };

    template <typename CT, typename NT>
    class TreeNode : public GraphNode<CT>
    {
    public:
        using NodeType = NT;
        using ConstNodeTypePtr = const NodeType*;
        using NodeTypePtr = NodeType*;

        ConstNodeTypePtr GetParent() const
        {
            return m_Parent;
        }
    protected:
        using GraphNode<CT>::GraphNode;

        NodeTypePtr m_Parent = nullptr;

        NodeTypePtr& GetParent()
        {
            return m_Parent;
        }
    };

    template <typename CT>
    class BinaryTreeNode : public TreeNode<CT, BinaryTreeNode<CT>>
    {
    public:
        using NodeType = BinaryTreeNode;
        using ConstNodeTypePtr = const NodeType*;
        using NodeTypePtr = NodeType*;

    public:
        ConstNodeTypePtr GetLeftChild() const
        {
            return m_LeftChild;
        }

        ConstNodeTypePtr GetRightChild() const
        {
            return m_RightChild;
        }

    protected:
        using TreeNode<CT, BinaryTreeNode<CT>>::TreeNode;

        NodeTypePtr m_LeftChild = nullptr;
        NodeTypePtr m_RightChild = nullptr;

        NodeTypePtr& GetLeftChild()
        {
            return m_LeftChild;
        }

        NodeTypePtr& GetRightChild()
        {
            return m_RightChild;
        }

        friend BinaryTree<BinaryTreeNode>;
        friend KdTree<BinaryTreeNode>;
    };

    template <typename NT>
    class Tree : public std::vector<NT*>
    {
    public:
        using NodeType = NT;
        using NodeTypePtr = NodeType*;

        NodeTypePtr GetRoot() const
        {
            return m_Root;
        }

        void Reset()
        {
            this->m_Root = nullptr;
            for (auto It = this->rbegin(); It != this->rend(); It++)
            {
                delete *It;
            }
            this->clear();
        }

        virtual ~Tree()
        {
            Reset();
        }
    protected:
        NodeTypePtr m_Root = nullptr;
    };

    template <typename NT>
    class BinaryTree : public Tree<NT>
    {
    public:
        using NodeType = NT;
        using NodeTypePtr = NodeType*;
        using NodePositionFn = std::function<NodeTypePtr&(NodeTypePtr)>;
        template <typename Position>
        struct NodePosition {
            static Position Get()
            {
                return Position();
            }
        };
        struct NodePositionLeft : public NodePosition<NodePositionLeft>, public NodePositionFn {
            using NodePositionFn::NodePositionFn;
            static NodePositionLeft Get()
            {
                return NodePositionLeft([](NodeTypePtr Node)->NodeTypePtr& {return Node->GetLeftChild();});
            }
        };
        struct NodePositionRight : public NodePosition<NodePositionLeft>, public NodePositionFn {
            using NodePositionFn::NodePositionFn;
            static NodePositionRight Get()
            {
                return NodePositionRight([](NodeTypePtr Node)->NodeTypePtr& {return Node->GetRightChild();});
            }
        };

        BinaryTree() = default;

        template <typename Position>
        NodeTypePtr AddNode(
            NodeType::ContentT Content,
            NodeTypePtr Parent,
            Position OnPosition = Position::Get()
        )
        {
            return AddNode_Impl(Content, Parent, OnPosition);
        }
    protected:
        NodeTypePtr AddNode_Impl(NodeType::ContentT Content, NodeTypePtr Parent, NodePositionFn OnPosition)
        {
            if (nullptr == Parent) {
                if (nullptr != this->m_Root)
                {
                    throw std::runtime_error("Cannot add node because parent is nullptr and tree already has root");
                }
                auto Root = new NodeType(Content);
                this->m_Root = Root;
                this->push_back(Root);
                return Root;
            } else {
                auto& Child = OnPosition(Parent);
                if (nullptr != Child)
                {
                    throw std::runtime_error("Cannot add child to parent because parent already has child");
                }
                Child = new NodeType(Content);
                auto ParentIt = std::find(this->begin(), this->end(), Parent);
                if (this->end() == ParentIt)
                {
                    throw std::runtime_error("Cannot add node because parent is not in tree");
                }
                Child->m_Parent = Parent;
                this->insert(++ParentIt, Child);
                return Child;
            }
        }
    };

    template <size_t Dimensions>
    size_t GetAxisIndexForKdTreeNode(size_t Depth)
    {
        return (size_t) std::trunc(Depth / Dimensions) % Dimensions;
    }

    template <typename NT>
    class KdTree : public BinaryTree<NT>
    {
    public:
        using NodeType = NT;
        using NodeTypePtr = NodeType*;
        using ConstNodeTypePtr = const NodeType*;

        KdTree() = default;

        template <size_t Dimensions, typename PositionT, bool AddLastNodes = true>
        void CreateKdTree(
            std::vector<typename NodeType::ContentT>& DataToBeOrdered,
            const std::function<PositionT(const typename NodeType::ContentT)>& GetPositionOfContent
        )
        {
            this->Reset();
            this->reserve(DataToBeOrdered.size());

            CreateKdTreeNode<typename BinaryTree<NT>::NodePositionLeft, PositionT, Dimensions, AddLastNodes>(
                DataToBeOrdered,
                GetPositionOfContent
            );
        }

        template <size_t Dimensions, typename PositionT>
        NodeTypePtr FindNode(
            const NodeType::ContentT& Content,
            const std::function<PositionT(const typename NodeType::ContentT)>& GetPositionOfContent,
            size_t& AtDepth,
            NodeTypePtr FromNode = nullptr,
            size_t Depth = Dimensions - 1
        )
        {
            if (nullptr == FromNode)
            {
                FromNode = this->m_Root;
            }

            if (FromNode->GetContent() == Content)
            {
                return FromNode;
            }

            auto Position = GetPositionOfContent(Content);
            auto NodePosition = GetPositionOfContent(FromNode->GetContent());
            auto OnIndex = GetAxisIndexForKdTreeNode<Dimensions>(Depth);
            if (Position[OnIndex] <= NodePosition[OnIndex] && nullptr != FromNode->GetLeftChild())
            {
                AtDepth = Depth;
                auto OnLeftNode = FindNode<Dimensions, PositionT>(
                    Content,
                    GetPositionOfContent,
                    FromNode->GetLeftChild(),
                    Depth + 1
                );
                if (nullptr != OnLeftNode || Position[OnIndex] < NodePosition[OnIndex]) {
                    return OnLeftNode;
                }
            }

            if (Position[OnIndex] >= NodePosition[OnIndex] && nullptr != FromNode->GetRightChild())
            {
                AtDepth = Depth;
                return FindNode<Dimensions, PositionT>(
                    Content,
                    GetPositionOfContent,
                    FromNode->GetRightChild(),
                    Depth + 1
                );
            }

            return nullptr;
        }

        template <size_t Dimensions, typename PositionT>
        NodeTypePtr FindNode(
            const NodeType::ContentT& Content,
            const std::function<PositionT(const typename NodeType::ContentT)>& GetPositionOfContent,
            NodeTypePtr FromNode = nullptr,
            size_t Depth = Dimensions - 1
        )
        {
            size_t AtDepth = 0;
            return FindNode<Dimensions, PositionT>(
                Content,
                GetPositionOfContent,
                AtDepth,
                FromNode,
                Depth
            );
        }

        template <size_t Dimensions>
        void DeleteNode(
            const NodeType::ContentT& Content
        )
        {
            size_t AtDepth = 0;
            auto Node = FindNode<Dimensions>(Content, AtDepth);
            if (nullptr == Node)
            {
                throw std::runtime_error("Node is not part of the tree");
            }

            // recreate tree from here
        }

    protected:
        template <typename PositionT>
        void SortNodeData(
            std::vector<typename NodeType::ContentT> Data,
            const std::function<PositionT(const typename NodeType::ContentT)>& GetPositionOfContent,
            const size_t& AxisIndex
        )
        {
            std::sort(Data.begin(), Data.end(), [&AxisIndex, &GetPositionOfContent](
                const NodeType::ContentT& Content1,
                const NodeType::ContentT& Content2
            ) {
                return GetPositionOfContent(Content1)[AxisIndex] < GetPositionOfContent(Content2)[AxisIndex];
            });
        }

        template <typename NodePosition, typename PositionT, size_t Dimensions, bool AddLastNodes = true>
        NodeTypePtr CreateKdTreeNode(
            std::vector<typename NodeType::ContentT> Data,
            const std::function<PositionT(const typename NodeType::ContentT)>& GetPositionOfContent,
            NodeTypePtr Parent = nullptr,
            // Dimensions - 1 to start with only 1 line and then cycle
            size_t Depth = Dimensions - 1
        )
        {
            const auto RestSize = Data.size();
            if (RestSize < 2)
            {
                if constexpr (AddLastNodes) {
                    if (RestSize == 1) {
                        this->template AddNode<NodePosition>(Data[0], Parent);
                    }
                }
                return nullptr;
            }

            const size_t SelectedIdx = Maths::Graph::GetAxisIndexForKdTreeNode<Dimensions>(Depth);
            SortNodeData(Data, GetPositionOfContent, SelectedIdx);

            const typename std::vector<typename NodeType::ContentT>::difference_type HalfSize = std::floor(RestSize / 2);
            const auto Index = HalfSize - 1;
            auto NewNode = this->template AddNode<NodePosition>(
                Data[Index],
                Parent
            );

            {
                std::vector<typename NodeType::ContentT> NewData;
                if (Index > 0) {
                    NewData.insert(NewData.begin(), Data.begin(), Data.begin() + Index);
                }
                CreateKdTreeNode<typename BinaryTree<NT>::NodePositionLeft, PositionT, Dimensions, AddLastNodes>(
                    NewData,
                    GetPositionOfContent,
                    NewNode,
                    Depth + 1
                );
            }
            {
                std::vector<typename NodeType::ContentT> NewData;
                if (Index + 1 < Data.size())
                {
                    NewData.insert(NewData.begin(), Data.begin() + Index + 1, Data.end());
                }
                CreateKdTreeNode<typename BinaryTree<NT>::NodePositionRight, PositionT, Dimensions, AddLastNodes>(
                    NewData,
                    GetPositionOfContent,
                    NewNode,
                    Depth + 1
                );
            }
            return NewNode;
        }
    };
} // Graph // Maths
