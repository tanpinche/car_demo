# FROM osrf/ros:kinetic-desktop

# RUN apt-get update \
#  && apt-get install -y \
#     wget \
#     lsb-release \
#     sudo \
#     mesa-utils \
#  && apt-get clean


FROM ros:kinetic-ros-core

# Get gazebo binaries
# RUN echo "deb http://packages.osrfoundation.org/gazebo/ubuntu `lsb_release -cs` main" > /etc/apt/sources.list.d/gazebo-stable.list \
#  && wget http://packages.osrfoundation.org/gazebo.key -O - | apt-key add - \
#  && apt-get update \
#  && apt-get install -y \
#     gazebo9 \
#     ros-kinetic-gazebo9-ros-pkgs \
#     ros-kinetic-fake-localization \
#     ros-kinetic-joy \
#  && apt-get clean

RUN apt-get update \ 
 && apt-get install -y wget \
 &&  sh -c 'echo "deb http://packages.osrfoundation.org/gazebo/ubuntu-stable `lsb_release -cs` main" > /etc/apt/sources.list.d/gazebo-stable.list' \
 && wget http://packages.osrfoundation.org/gazebo.key -O - | apt-key add - \
 && apt-get update \
 && apt-get install -y \
    build-essential \
    gazebo9 \
    ros-kinetic-gazebo9-ros-pkgs \
    ros-kinetic-fake-localization \
    ros-kinetic-joy \
    ros-kinetic-gazebo9-ros-control \
    ros-kinetic-ros-controllers \
    ros-kinetic-joint-state-publisher \
    ros-kinetic-octomap \
    ros-kinetic-costmap-2d \
    ros-kinetic-map-server \
    ros-kinetic-robot-state-publisher \
    ros-kinetic-rviz


# RUN mkdir -p /tmp/workspace/src
# COPY prius_description /tmp/workspace/src/prius_description
# COPY prius_msgs /tmp/workspace/src/prius_msgs
# COPY car_demo /tmp/workspace/src/car_demo
# RUN /bin/bash -c 'cd /tmp/workspace \
#  && source /opt/ros/kinetic/setup.bash \
#  && catkin_make'

WORKDIR /home/sim_ws
RUN mkdir src 


#  && /ros_entrypoint.sh catkin_make 
COPY /kopilot_launch src/kopilot_launch
COPY /adapter src/adapter
COPY /prius_description src/prius_description
COPY /prius_msgs src/prius_msgs
COPY /car_demo src/car_demo
COPY /gazebo_ros_2Dmap_plugin src/gazebo_ros_2Dmap_plugin
COPY ros_entrypoint.sh .


RUN /ros_entrypoint.sh catkin_make && \
    sed -i '$isource "/home/sim_ws/devel/setup.bash"' /ros_entrypoint.sh 
RUN echo "export GAZEBO_MODEL_PATH=$GAZEBO_MODEL_PATH:/home/sim_ws/src/car_demo/models:$GAZEBO_MODEL_PATH" >> ~/.bashrc

# RUN rosdep init

# RUN rosdep update
# WORKDIR /home/sim_ws/src/

ENTRYPOINT ["/ros_entrypoint.sh"]

CMD roslaunch kopilot_launch ackermann_sim.launch


# CMD ["/bin/bash", "-c", "source /opt/ros/kinetic/setup.bash && source /tmp/workspace/devel/setup.bash && roslaunch car_demo demo.launch"]


