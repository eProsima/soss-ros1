/*
 * Copyright (C) 2019 Open Source Robotics Foundation
 * Copyright (C) 2020 - present Proyectos y Sistemas de Mantenimiento SL (eProsima).
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include "MetaPublisher.hpp"

#include <is/sh/ros1/Factory.hpp>

#include <is/core/runtime/StringTemplate.hpp>

#include <unordered_map>

namespace eprosima {
namespace is {
namespace sh {
namespace ros1 {

//==============================================================================
class MetaPublisher : public is::TopicPublisher
{
public:

    MetaPublisher(
            core::StringTemplate&& topic_template,
            const eprosima::xtypes::DynamicType& message_type,
            ros::NodeHandle& node,
            uint32_t queue_size,
            bool latch,
            const YAML::Node& /*unused*/)
        : _topic_template(std::move(topic_template))
        , _message_type(message_type)
        , _node(node)
        , _queue_size(queue_size)
        , _latch(latch)
    {
        // Do nothing
    }

    bool publish(
            const eprosima::xtypes::DynamicData& message) override final
    {
        const std::string topic_name = _topic_template.compute_string(message);

        const auto insertion = _publishers.insert(
            std::make_pair(std::move(topic_name), nullptr));
        const bool inserted = insertion.second;
        TopicPublisherPtr& publisher = insertion.first->second;

        if (inserted)
        {
            publisher = Factory::instance().create_publisher(
                _message_type, _node, topic_name, _queue_size, _latch);
        }

        return publisher->publish(message);
    }

private:

    const core::StringTemplate _topic_template;
    const eprosima::xtypes::DynamicType& _message_type;
    ros::NodeHandle& _node;
    const uint32_t _queue_size;
    const bool _latch;

    using TopicPublisherPtr = std::shared_ptr<TopicPublisher>;
    using PublisherMap = std::unordered_map<std::string, TopicPublisherPtr>;
    PublisherMap _publishers;

};

namespace  {
//==============================================================================
std::string make_detail_string(
        const std::string& topic_name,
        const std::string& message_type)
{
    return
        "[Middleware: ROS1, topic template: "
        + topic_name + ", message type: " + message_type + "]";
}

} // anonymous namespace

//==============================================================================
std::shared_ptr<is::TopicPublisher> make_meta_publisher(
        const eprosima::xtypes::DynamicType& message_type,
        ros::NodeHandle& node,
        const std::string& topic_name,
        uint32_t queue_size,
        bool latch,
        const YAML::Node& configuration)
{
    return std::make_shared<MetaPublisher>(
        core::StringTemplate(topic_name, make_detail_string(topic_name, message_type.name())),
        message_type, node, queue_size, latch, configuration);
}

} //  namespace ros1
} //  namespace sh
} //  namespace is
} //  namespace eprosima
