#include "../stdafx.h"
#include <stdexcept>
#include "TunaNode.h"
namespace tunage{
        void TunaNode::render()
        {
            throw std::runtime_error("render() called on a pure TunaNode object");
        }

        void TunaNode::setMatrix(glm::mat4 matrix)
        {
            m_matrix = matrix;
        }

        void TunaNode::setParent(TunaNode* parent)
        {
            this->m_parent = parent;
        }

        TunaNode* TunaNode::getParent()
        {
            return m_parent;
        }

        std::vector<TunaNode*> TunaNode::getChildren(){
            return m_hierarchy;
        }

        void TunaNode::link(TunaNode* child)
        {
            child->setParent(this);
            m_hierarchy.push_back(child);
        }

        TunaNode* TunaNode::unlinkById(int id)
        {
            for (auto i = m_hierarchy.begin(); i != m_hierarchy.end(); ++i) {
                if ((*i)->getId() == id) {
                    (*i)->setParent(nullptr);
                    TunaNode *node = *i;
                    m_hierarchy.erase(i);
                    return node;
                }
            }
            return nullptr;
        }

    glm::mat4 TunaNode::getMatrix() {
        return m_matrix;
    }
};