#pragma once
#include <filesystem>
#include <format>
#include <fstream>
#include <string>
#include <vector>

namespace svg {
using Point = std::pair<double, double>;
struct Circle {
    Circle(double radius, Point center, std::string color)
        : radius(radius),
          center(center),
          color(std::move(color)) {}
    double radius;
    Point center;
    std::string color;
};
struct Line {
    Line(Point start, Point end, double width, std::string color)
        : start(start),
          end(end),
          width(width),
          color(std::move(color)) {}

    Point start, end;
    double width;
    std::string color;
};

class SVGCreator {
    constexpr static double s_multiplier = 100.0;
  public:
    void addLine(Line line) {
        min_point.first = std::min(min_point.first, line.start.first - line.width);
        min_point.first = std::min(min_point.first, line.end.first - line.width);
        min_point.second = std::min(min_point.second, line.start.second - line.width);
        min_point.second = std::min(min_point.second, line.end.second - line.width);

        max_point.first = std::max(max_point.first, line.start.first + line.width);
        max_point.first = std::max(max_point.first, line.end.first + line.width);
        max_point.second = std::max(max_point.second, line.start.second + line.width);
        max_point.second = std::max(max_point.second, line.end.second + line.width);

        m_lines.push_back(std::move(line));
    }
    void addCircle(Circle circle) {
        min_point.first = std::min(min_point.first, circle.center.first - circle.radius);
        min_point.second = std::min(min_point.second, circle.center.second - circle.radius);
        max_point.first = std::max(max_point.first, circle.center.first + circle.radius);
        max_point.second = std::max(max_point.second, circle.center.second + circle.radius);

        m_circles.push_back(circle);
    }
    void saveToFile(const std::filesystem::path& file) {
        std::ofstream out(file);
        long width = std::ceil((max_point.first - min_point.first) * s_multiplier);
        long height = std::ceil((max_point.second - min_point.second) * s_multiplier);
        out << std::format("<svg width=\"{}\" height=\"{}\" xmlns=\"http://www.w3.org/2000/svg\">\n", width, height);

        for (const auto& circle : m_circles)
            out << std::format("<circle r=\"{}\" cx=\"{}\" cy=\"{}\" fill=\"{}\" />\n",
                               long(circle.radius * s_multiplier),
                               long(circle.center.first * s_multiplier),
                               long(height - circle.center.second * s_multiplier),
                               circle.color);

        for (const auto& line : m_lines)
            out << std::format("<line x1=\"{}\" y1=\"{}\" x2=\"{}\" y2=\"{}\" style=\"stroke:{};stroke-width:{}\"/>\n",
                               long(line.start.first * s_multiplier),
                               long(height - line.start.second * s_multiplier),
                               long(line.end.first * s_multiplier),
                               long(height - line.end.second * s_multiplier),
                               line.color,
                               long(line.width * s_multiplier));

        out << "</svg>\n";
    }

  private:
    std::vector<Circle> m_circles;
    std::vector<Line> m_lines;

    constexpr static double DMAX = std::numeric_limits<double>::max();
    constexpr static double DMIN = std::numeric_limits<double>::lowest();
    std::pair<double, double> min_point = {DMAX, DMAX};
    std::pair<double, double> max_point = {DMIN, DMIN};
};
} // namespace svg