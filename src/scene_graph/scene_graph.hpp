//
// Created by Gianni on 27/01/2025.
//

#ifndef VULKANRENDERINGENGINE_SCENE_GRAPH_HPP
#define VULKANRENDERINGENGINE_SCENE_GRAPH_HPP

#include "../app/simple_notification_service.hpp"
#include "mesh_node.hpp"

class SceneGraph : public SubscriberSNS
{
public:
    SceneGraph();

    void addNode(GraphNode* node);
    void deleteNode(uuid32_t nodeID);
    void updateTransforms();

    void notify(const Message &message) override;

    GraphNode* searchNode(uuid32_t nodeID);
    bool hasDescendant(GraphNode* current, GraphNode* descendant);

public:
    GraphNode mRoot;
};

#endif //VULKANRENDERINGENGINE_SCENE_GRAPH_HPP
