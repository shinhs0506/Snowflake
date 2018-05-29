/*
 * Created By: Robyn Castro
 * Created On: November 10th, 2017
 * Description: Utilities to create Rviz visualisation messages
 *
 */
#include <RvizUtils.h>

using namespace std;
using namespace visualization_msgs;
using namespace snowbots;

Marker RvizUtils::createMarker(vector<geometry_msgs::Point> points,
                               Marker::_color_type color,
                               Marker::_scale_type scale,
                               string frame_id,
                               string ns,
                               int type,
                               int id) {
    Marker marker;

    setupMarker(marker, scale, frame_id, ns, type, id);

    // Set the color
    marker.color = color;

    // TODO: There are similar functions in this file that we should do this in....
    // Set the origin to the first point and offset the rest of points relative
    // to the first point
    // We do this because points at really large x/y values don't seem to
    // visualise very well in RViz
    if (points.size() > 0){
        geometry_msgs::Point origin = points[0];
        marker.pose.position.x = origin.x;
        marker.pose.position.y = origin.y;
        marker.pose.position.z = origin.z;

        points = offsetPointsRelativeToPoint(origin, points);
    }

    // Set the points
    marker.points = points;

    return marker;
}

Marker RvizUtils::createMarker(vector<geometry_msgs::Point> points,
                               std::vector<std_msgs::ColorRGBA> colors,
                               Marker::_scale_type scale,
                               string frame_id,
                               string ns,
                               int type,
                               int id) {
    Marker marker;

    setupMarker(marker, scale, frame_id, ns, type, id);

    // Set the colors
    marker.colors = colors;

    // Set the points
    marker.points = points;

    return marker;
}

Marker RvizUtils::createMarker(geometry_msgs::Point point,
                               std::vector<std_msgs::ColorRGBA> colors,
                               Marker::_scale_type scale,
                               string frame_id,
                               string ns,
                               int type,
                               int id) {
    Marker marker;

    setupMarker(marker, scale, frame_id, ns, type, id);

    // Set the color
    marker.colors = colors;

    // Set the points
    marker.points.push_back(point);

    return marker;
}

MarkerArray
RvizUtils::createMarkerArray(vector<vector<geometry_msgs::Point>> points_array,
                             Marker::_color_type color,
                             Marker::_scale_type scale,
                             string frame_id,
                             string ns,
                             int type) {
    visualization_msgs::MarkerArray markerArray;
    for (unsigned int i = 0; i < points_array.size(); i++) {
        Marker marker =
        createMarker(points_array[i], color, scale, frame_id, ns, type, i);
        markerArray.markers.push_back(marker);
    }

    return markerArray;
}

Marker::_color_type
RvizUtils::createMarkerColor(float r, float g, float b, float a) {
    Marker::_color_type color;
    color.r = r;
    color.g = g;
    color.b = b;
    color.a = a;

    return color;
}

Marker::_scale_type RvizUtils::createrMarkerScale(float x, float y, float z) {
    Marker::_scale_type scale;
    scale.x = x;
    scale.y = y;
    scale.z = z;

    return scale;
}

void RvizUtils::setupMarker(Marker& marker,
                            visualization_msgs::Marker::_scale_type scale,
                            std::string frame_id,
                            std::string ns,
                            int type,
                            int id) {
    marker.header.stamp       = ros::Time::now();
    marker.action             = Marker::ADD;
    marker.pose.orientation.w = 1.0;

    marker.type = type;
    marker.id   = id;

    marker.header.frame_id = frame_id;
    marker.ns              = ns;

    marker.scale = scale;

    // TODO: Make this an option (default true)
    marker.frame_locked = true;
}

std::vector<geometry_msgs::Point> RvizUtils::offsetPointsRelativeToPoint(geometry_msgs::Point origin, std::vector<geometry_msgs::Point> points) {
    for (geometry_msgs::Point& point : points){
        point.x -= origin.x;
        point.y -= origin.y;
        point.z -= origin.z;
    }
    return points;
}
