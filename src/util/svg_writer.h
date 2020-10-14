#ifndef SVG_WRITER_H
#define SVG_WRITER_H
#include <string>
#include <vector>

#include <iostream>
#include <fstream>
#include <math.h>

namespace svg {

class Vector2D
{
public:
    double _x;
    double _y;

    Vector2D(){}

    Vector2D(double x, double y) : _x(x), _y(y){}
    void operator+=(const Vector2D& v)
    {
        _x += v._x;
        _y += v._y;
    }

    void operator-=(const Vector2D& v)
    {
        _x -= v._x;
        _y -= v._y;
    }

    void operator*=(double scale)
    {
        _x *= scale;
        _y *= scale;
    }
};

static Vector2D operator+(const Vector2D &p1, const Vector2D &p2)
{
    Vector2D v = p1;
    v += p2;
    return v;
}

static Vector2D operator-(const Vector2D &p1, const Vector2D &p2)
{
    Vector2D v = p1;
    v -= p2;
    return v;
}

static Vector2D operator*(double scale, const Vector2D &p1)
{
    Vector2D v = p1;
    v *= scale;
    return v;
}

static Vector2D operator*(const Vector2D &p1, double scale)
{
    Vector2D v = p1;
    v *= scale;
    return v;
}


class Color
{
public:
    int _r{0};
    int _g{0};
    int _b{0};
    Color(){}
    Color(int r, int g, int b) : _r(r), _g(g), _b(b){}
};

class SvgObject
{
protected:
    Vector2D _pos;
    Color _fill;

    SvgObject(){}
    SvgObject(Vector2D pos, Color& color) : _pos(pos), _fill(color){}

    Vector2D calcRenderPosition(const Vector2D &bb_center, double scale, const Vector2D &svg_center)
    {
        return calcRenderPosition(_pos, bb_center, scale, svg_center);
    }

    Vector2D calcRenderPosition(const Vector2D &pos, const Vector2D &bb_center, double scale, const Vector2D &svg_center)
    {
        auto tp = (pos - bb_center) * scale;
        tp._y = -1.0 * tp._y;
        tp += svg_center;
        return tp;
    }

    std::string xy(double x, double y)
    {
        return "x=\"" + std::to_string(x) + "\" y=\"" + std::to_string(y) + "\"";
    }

    std::string xyComma(Vector2D &p)
    {
        return std::to_string(p._x) + "," + std::to_string(p._y);
    }

    std::string xyCommaArray(std::vector<Vector2D> &points)
    {
        std::string text;
        auto size = points.size();
        for(auto i = 0; i < size; i++)
        {
            text += xyComma(points[i]);
            if (i != size - 1)
            {
                text += " ";
            }
        }
        return text;
    }

    std::string points(std::vector<Vector2D> &points)
    {
        std::string text = "points=\"" + xyCommaArray(points) + "\"";
        return text;
    }

    std::string center(double x, double y)
    {
        return "cx=\"" + std::to_string(x) + "\" cy=\"" + std::to_string(y) + "\"";
    }

    std::string style()
    {
        auto r = _fill._r;
        auto g = _fill._g;
        auto b = _fill._b;
        std::string text = "style=\"fill:rgb(" + std::to_string(r) + "," + std::to_string(g) + "," + std::to_string(b) + ")\"";
        return text;
    }

public:
    virtual std::string render(Vector2D &bb_center, double scale, Vector2D& svg_center) = 0;
};

class Rect : public SvgObject
{
public:
    double _w;
    double _h;

    Rect(double x, double y, double w, double h, Color& color) : SvgObject(Vector2D(x,y),color), _w(w), _h(h)
    {
    }

    std::string render(Vector2D &bb_center, double scale, Vector2D& svg_center)
    {
        auto p = calcRenderPosition(bb_center, scale, svg_center);

        std::string wh = "width=\"" + std::to_string(_w) + "\" height=\"" + std::to_string(_h) + "\"";
        std::string  text = "<rect " + xy(p._x, p._y) + " " + wh + " " + style() + " />\n";
        return text;
    }
};

class Circle : public SvgObject
{
public:
    double _radius;
    Circle(double x, double y, double radius, Color& color) : SvgObject(Vector2D(x,y),color), _radius(radius)
    {
    }

    std::string render(Vector2D &bb_center, double scale, Vector2D& svg_center)
    {
        auto p = calcRenderPosition(bb_center, scale, svg_center);

        std::string rad = "r=\"" + std::to_string(_radius) + "\"";
        std::string text = "<circle " + center(p._x, p._y) + " " + rad + " " + style() + " />\n";
        return text;
    }
};

class Triangle : public SvgObject
{
    double _size;
    //std::vector<Vector2D> _points;

public:
    Triangle(double x, double y, double size, Color& color) : SvgObject(Vector2D(x,y),color), _size(size)
    {

    }

    std::string render(Vector2D &bb_center, double scale, Vector2D& svg_center)
    {
        auto p = calcRenderPosition(bb_center, scale, svg_center);

        auto dx = _size*sqrt(3)*0.5;
        auto dy = _size*0.5;

        std::vector<Vector2D> render_points;
        render_points.resize(3);
        render_points[0] = Vector2D(p._x, p._y + _size);
        render_points[1] = Vector2D(p._x - dx, p._y - dy);
        render_points[2] = Vector2D(p._x + dx, p._y - dy);

        std::string text = "<polygon " + points(render_points) + " " + style() + " />";
        return text;
    }
};

class Polyline : public SvgObject
{
    std::vector<Vector2D> _points;
    Color stroke;
    //<polyline points="20,20 40,25 60,40 80,120 120,140 200,180" style="fill:none;stroke:black;stroke-width:3" />
public:

    Polyline(std::vector<Vector2D>& pt_arr, Color color) :  SvgObject(), _points(pt_arr), stroke(color)
    {
    }

    Polyline(std::vector<double>& xy_arr, Color color) :  SvgObject(), stroke(color)
    {
        auto size = xy_arr.size()/2;
        for(auto i = 0; i < size; i++)
        {
            auto x = xy_arr[2*i];
            auto y = xy_arr[2*i+1];
            _points.push_back(Vector2D(x,y));
        }
    }

    std::string render(Vector2D &bb_center, double scale, Vector2D& svg_center)
    {
        std::vector<Vector2D> render_points;
        for(int i = 0; i < _points.size(); i++)
        {
            auto p = calcRenderPosition(_points[i], bb_center, scale, svg_center);
            render_points.push_back(p);
        }

        return "<polyline " + points(render_points) +" style=\"fill:none;stroke:black;stroke-width:1\" \/>";
    }
};

class SvgWriter
{
private:
    std::vector<SvgObject*> _obj;
    int _svg_w;
    int _svg_h;

    double _left;
    double _top;
    double _right;
    double _bottom;
    bool _b_first_bb{true};

public:
    SvgWriter(int w, int h) : _svg_w(w), _svg_h(h){}

    ~SvgWriter()
    {
        for(auto i = 0; i < _obj.size(); i++)
        {
            delete _obj[i];
        }
    }

    void expandBB(double x, double y)
    {
        if(_b_first_bb)
        {
            _left = x;
            _right = x;
            _top = y;
            _bottom = y;
            _b_first_bb = false;
        }
        else
        {
            if(x < _left)
                _left = x;

            if(_right < x)
                _right = x;

            if(y < _bottom)
                _bottom = y;

            if(_top < y)
                _top = y;
        }
    }

    void addRect(double x, double y, double w, double h, Color& fill)
    {
        auto rect = new Rect(x,y,w,h,fill);
        _obj.push_back(rect);
        expandBB(x,y);
    }

    void addCircle(double x, double y, double radius, Color& fill)
    {
        auto circle = new Circle(x,y,radius,fill);
        _obj.push_back(circle);
        expandBB(x,y);
    }

    void addTriangle(double x, double y, double size, Color& fill)
    {
        auto triangle = new Triangle(x,y, size, fill);
        _obj.push_back(triangle);
        expandBB(x,y);
    }

    void writeFile(std::string file_path)
    {
        auto bb_x = _right - _left;
        auto bb_y = _top - _bottom;
        auto scale_w = _svg_w/bb_x;
        auto scale_h = _svg_h/bb_y;

        auto scale = scale_w < scale_h ? scale_w : scale_h;
        scale *= 0.9; // consider margin

        auto bb_cx = 0.5*(_right + _left);
        auto bb_cy = 0.5*(_top + _bottom);

        auto svg_cx = _svg_w*0.5;
        auto svg_cy = _svg_h*0.5;

        auto bb_center = Vector2D(bb_cx, bb_cy);
        auto svg_center = Vector2D(svg_cx, svg_cy);

        std::ofstream html_file;
        html_file.open (file_path);

        html_file << "<!DOCTYPE html>\n<html>\n<body>\n<svg height=\"" + std::to_string(_svg_h) + "\" width=\"" + std::to_string(_svg_w) +"\" style=\"background-color:rgb(245,253,255)\">\n";

        for(auto i = 0; i < _obj.size(); i++)
        {
            html_file << _obj[i]->render(bb_center, scale, svg_center);
        }

        auto axis_expand_scale = 1.05;
        std::vector<Vector2D> axis_points;
        axis_points.push_back(Vector2D(0,_top * axis_expand_scale));
        axis_points.push_back(Vector2D(0,0));
        axis_points.push_back(Vector2D(_right * axis_expand_scale,0));

        auto axis = Polyline(axis_points, Color(100,100,100));
        html_file << axis.render(bb_center, scale, svg_center);

        /*
        std::vector<Vector2D> rect_points;
        rect_points.push_back(Vector2D(_left,_top));
        rect_points.push_back(Vector2D(_left, _bottom));
        rect_points.push_back(Vector2D(_right, _bottom));
        rect_points.push_back(Vector2D(_right, _top));

        auto rect = Polyline(rect_points, Color(200,200,200));
        html_file << rect.render(bb_center, scale, svg_center);
        */

        html_file << "</svg>\n</body>\n</html>";
        html_file.close();
    }
};

}

#endif // SVG_WRITER_H
