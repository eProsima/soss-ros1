<a href="https://integration-service.docs.eprosima.com/"><img src="https://github.com/eProsima/Integration-Service/blob/main/docs/images/logo.png?raw=true" hspace="8" vspace="2" height="100" ></a>

# ROS 1 System Handle

[![ROS 1 SH CI Status](https://github.com/eProsima/ROS1-SH/actions/workflows/ci.yml/badge.svg)](https://github.com/eProsima/ROS1-SH/actions)
&nbsp;&nbsp;[![Have an issue?](https://img.shields.io/badge/-Open%20an%20issue-orange)](https://github.com/eProsima/Integration-Service/issues/new)

## Introduction

### What is a System Handle?

A [System Handle](https://integration-service.docs.eprosima.com/en/latest/user_manual/systemhandle/sh.html) is a plugin that allows a certain middleware
or communication protocol to speak the same language used by the [eProsima Integration Service](https://github.com/eProsima/Integration-Service),
that is, *Extensible and Dynamic Topic Types for DDS* (**xTypes**);
specifically, *Integration Service* bases its intercommunication abilities on eProsima's open source
implementation for the *xTypes* protocol, that is, [eProsima xTypes](https://github.com/eProsima/xtypes).

<p align="center">
  <a href="https://integration-service.docs.eprosima.com/en/latest/user_manual/systemhandle/sh.html"><img src="docs/images/system-handle-architecture.png"></a>
</p>

### The ROS 1 SystemHandle

<a href="https://wiki.ros.org/noetic"><img src="docs/images/ros1_logo.png" align="left" hspace="4" vspace="2" width="140"></a>

This repository contains the source code of *Integration Service* **System Handle**
for the [ROS 1](https://wiki.ros.org/noetic) middleware protocol, widely used in the robotics field.

The main purpose of the *ROS 1 System Handle* is that of establishing a connection between a *ROS 1*
application and an application running over a different middleware implementation.
This is the classic use-case approach for *Integration Service*.

## Dependencies

This section provides a list of the dependencies needed in order to compile *ROS 1 System Handle*.

* [ROS 1](http://wiki.ros.org/ROS/Installation): *Melodic/Noetic ROS 1* distribution.

## Configuration

*Integration Service* is configured by means of a YAML configuration file, which specifies
the middlewares, topics and/or services involved in the intercommunication process, as well as
their topic/service types and the data exchange flow. This configuration file is loaded during
runtime, so there is no need to recompile any package before switching to a whole new
intercommunication architecture.

To get a more precise idea on how these YAML files have to be filled and which fields they require
in order to succesfully configure and launch an *Integration Service* project, please refer to the
[dedicated configuration section](https://integration-service.docs.eprosima.com/en/latest/user_manual/yaml_config.html) of the official documentation.

Regarding the *ROS 1 System Handle*, there are several specific parameters which can be configured
for the ROS 1 middleware. All of these parameters are optional, and fall as suboptions of the main
five sections described in the *Configuration* chapter of *Integration Service* repository:

* `systems`: The system `type` must be `ros1`. In addition to the `type` and `types-from` fields,
  the *ROS 1 System Handle* accepts the following specific configuration fields:

  ```yaml
  systems:
    ros1:
      type: ros1
      node_name: "my_ros1_node"
  ```
  * `node_name`: The *ROS 1 System Handle* node name.

* `topics`: The topic `route` must contain `ros1` within its `from` or `to` fields. Additionally,
  the *ROS 1 System Handle* accepts the following topic specific configuration parameters, within the
  `ros1` specific middleware configuration tag:

  ```yaml
  routes:
    ros2_to_ros1: { from: ros2, to: ros1 }
    ros1_to_dds: { from: ros1, to: dds }

  topics:
    hello_ros1:
      type: std_msgs/String
      route: ros2_to_ros1
      ros1: { queue_size: 10, latch: false }
    hello_dds:
      type: std_msgs/String
      route: ros1_to_dds
      ros1: { queue_size: 5 }
  ```

  * `queue_size`: The maximum message queue size for the ROS 1 publisher or subscription.
  * `latch`: Enable or disable latching. When a connection is latched,
    the last message published is saved and sent to any future subscribers that connect.
    This configuration parameter only makes sense for ROS 1 publishers, so it is only useful for
    routes where the *ROS 1 System Handle* acts as a publisher, that is, for routes where `ros1` is
    included in the `to` list.
## Examples

There are several *Integration Service* examples using the *ROS 1 System Handle* available
in the project's [main source code repository]([https://](https://github.com/eProsima/Integration-Service/tree/main/examples)).

One of these examples, where the *ROS 1 System Handle* takes part in the intercommunication process, is introduced here.

<a href="https://integration-service.docs.eprosima.com/en/latest/examples/different_protocols/pubsub/ros1-ros2.html"><img align="left" width="15" height="38" src="https://via.placeholder.com/15/40c15d/000000?text=+" alt="Green icon"></a>

### ROS 1 - ROS 2 bridge  (publisher - subscriber)

In this example, *Integration Service* uses both this *ROS 1 System Handle* and the *ROS 2 System Handle*
to transmit data coming from a ROS 1 publisher into the ROS 2 data space, so that it can be
consumed by a ROS 2 subscriber on the same topic, and viceversa.

<p align="center">
  <a href="https://integration-service.docs.eprosima.com/en/latest/examples/different_protocols/pubsub/ros1-ros2.html"><img src="docs/images/ros1_ros2_pubsub_example.png" width="500"></a>
</p>

The configuration file used by *Integration Service* for this example can be found
[here](https://github.com/eProsima/Integration-Service/blob/main/examples/basic/ros1_ros2__helloworld.yaml).

For a detailed step by step guide on how to build and test this example, please refer to the
[dedicated section](https://integration-service.docs.eprosima.com/en/latest/examples/different_protocols/pubsub/ros1-ros2.html) in the official documentation.


<a href="https://integration-service.docs.eprosima.com/en/latest/examples/different_protocols/services/ros1-server.html"><img align="left" width="15" height="38" src="https://via.placeholder.com/15/40c15d/000000?text=+" alt="Green icon"></a>

### ROS 1 service server

In this example, the *ROS 1 System Handle* tackles the task of bridging a ROS 1 server with one or more client applications,
playing the role of a service server capable of processing incoming requests from several middlewares (*DDS*, *ROS2*,
*WebSocket*) and producing an appropriate answer for them.

<p align="center">
  <a href="https://integration-service.docs.eprosima.com/en/latest/examples/different_protocols/services/ros1-server.html"><img src="docs/images/ros1_services_example.png" width="550"></a>
</p>

The configuration file used by *Integration Service* for this example can be found
[here](https://github.com/eProsima/Integration-Service/blob/main/examples/basic/ros1_server__addtwoints.yaml).

For a detailed step by step guide on how to build and test this example, please refer to the
[dedicated section](https://integration-service.docs.eprosima.com/en/latest/examples/different_protocols/services/ros1-server.html) in the official documentation.
## Compilation flags

Besides the [global compilation flags](https://integration-service.docs.eprosima.com/en/latest/installation_manual/installation.html#global-compilation-flags) available for the
whole *Integration Service* product suite, there are some specific flags which apply only to the
*ROS 1 System Handle*; they are listed below:

* `BUILD_ROS1_TESTS`: Allows to specifically compile the *ROS 1 System Handle* unitary and
  integration tests; this is useful to avoid compiling each *System Handle's* test suite present
  in the `colcon` workspace, which is what would happen if using the `BUILD_TESTS` flag; and thus,
  minimizing the building time; to use it, after making sure that the *ROS 1 System Handle*
  is present in the `colcon` workspace, the following command must be executed:
  ```bash
  ~/is_ws$ colcon build --cmake-args -DBUILD_ROS1_TESTS=ON
  ```

* `MIX_ROS_PACKAGES`: It accepts as an argument a list of [ROS packages](https://index.ros.org/packages/),
  such as `std_msgs`, `geometry_msgs`, `sensor_msgs`, `nav_msgs`... for which the required transformation
  library to convert the specific ROS 1 type definitions into *xTypes*, and the other way around, will be built.
  This list is shared with the [ROS 2 System Handle](https://github.com/eProsima/ROS2-SH#compilation-flags),
  meaning that the ROS packages specified in the `MIX_ROS_PACKAGES` variable will also be built for *ROS 2*
  if the corresponding *System Handle* is present within the *Integration Service* workspace.
  To avoid possible errors, if a certain package is only present in *ROS 1*, the `MIX_ROS1_PACKAGES` flag must be used instead.

  These transformation libraries are also known within the *Integration Service* context as `Middleware Interface Extension`
  or `mix` libraries.

  By default, only the `std_msgs_mix` library is compiled, unless the `BUILD_TESTS`
  or `BUILD_ROS1_TESTS` is used, case in which some additional ROS 1 packages `mix` files
  required for testing will be built.

  If the user wants to compile some additional packages to use them with *Integration Service*,
  the following command must be launched to compile it, adding as much packages to the list as desired:
  ```bash
  ~/is_ws$ colcon build --cmake-args -DMIX_ROS_PACKAGES="std_msgs geometry_msgs sensor_msgs nav_msgs"
  ```

* `MIX_ROS1_PACKAGES`: It is used just as the `MIX_ROS_PACKAGES` flag, but will only affect *ROS 1*;
  this means that the `mix` generation engine will not search within the *ROS 2* packages,
  allowing to compile specific *ROS 1* packages independently.

  For example, if a user wants to compile a certain package `dummy_msgs` independently from *ROS 1*,
  but compiling `std_msgs` and `geometry_msgs` for both the *ROS 1* and *ROS 2 System Handles*,
  the following command should be executed:
  ```bash
  ~/is_ws$ colcon build --cmake-args -DMIX_ROS_PACKAGES="std_msgs geometry_msgs" -DMIX_ROS1_PACKAGES="dummy_msgs"
  ```

## Documentation

The official documentation for the *ROS 1 System Handle* is included within the official *Integration Service*
documentation, hosted by [Read the Docs](https://integration-service.docs.eprosima.com/), and comprises the following sections:

* [Installation Manual](https://integration-service.docs.eprosima.com/en/latest/installation_manual/installation_manual.html)
* [User Manual](https://integration-service.docs.eprosima.com/en/latest/user_manual/systemhandle/ros1_sh.html)
* [API Reference](https://integration-service.docs.eprosima.com/en/latest/api_reference/ros1_sh/api_is_ros1_sh.html)

## License

This repository is open-sourced under the *Apache-2.0* license. See the [LICENSE](LICENSE) file for more details.

## Getting help

If you need support you can reach us by mail at `support@eProsima.com` or by phone at `+34 91 804 34 48`.
