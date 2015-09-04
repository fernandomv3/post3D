#ifndef COLOR_H
#define COLOR_H
#include <memory>
namespace math3D{
/** 
 *  Represents a RGBA color.
 *  Each channel is stored in a float member clamped between 0.0f and 1.0f
 */
class Color{
private:
  float r;/**< Red channel. Stores the red channel value using a float between 0.0f and 1.0f. */
  float g;/**< Green channel. Stores the green channel value using a float between 0.0f and 1.0f. */
  float b;/**< Blue channel. Stores the blue channel value using a float between 0.0f and 1.0f. */
  float a;/**< Alpha channel. Stores the alpha channel value using a float between 0.0f and 1.0f. */
public:
  /**
   * Color constructor.
   * The constructor needs every channel to be specified, all channels will be clamped to [0.0f..1.0f]
   * @param r Red channel
   * @param g Green channel
   * @param b Blue channel
   * @param a Alpha channel
   */
  Color(float r, float g, float b, float a);
  /**
   * Default Color constructor.
   * The constructor will set every channel to 0.0f.
   */
  Color()=default;
  /**
   * Color copy constructor.
   */
  Color(const Color& color)=default;
  /**
   * Color move constructor.
   */
  Color(Color&& color)=default;
  /**
   * Color copy assignment.
   */
  Color& operator=(const Color& color)=default;
  /**
   * Color move assignment.
   */
  Color& operator=(Color&& color)=default;
  /**
   * Color destructor.
   * No pointers allocated, the default generated constructor is used.
   */
  ~Color()=default;
  /**
   * Set all channels.
   * Every channel is set with ints in range from 0 to 255. Values outside the range will be clamped
   * @param r Red channel.
   * @param g Green channel.
   * @param b Blue channel.
   * @param a Alpha channel.
   * @return Returns the Color object.
   */
  Color& setRGBA255(int r = 0, int g = 0, int b = 0, int a = 1);
  /**
   * Set all channels.
   * Every channel is set with ints in range from 0.0f to 1.0f. Values outside the range will be clamped
   * @param r Red channel.
   * @param g Green channel.
   * @param b Blue channel.
   * @param a Alpha channel.
   * @return Returns the Color object.
   */
  Color& setRGBA(float r = 0.0, float g = 0.0, float b = 0.0, float a = 1.0);
  /**
   * Set a specific channel.
   * Sets a float value between 0.0f and 1.0f to a specific channel
   * @param component A char element that can take the values: 'r','g','b','a'. Other values will do nothing.
   * @param value The value to be set for the channel
   * @return Returns the Color object.
   */
  Color& setComponent(char component, float value);
  /**
   * Add Operator
   * Adds each channel respectively. Results are clamped between 0.0f and 1.0f.
   * @param color The color to be added
   * @return Returns the Color object after the add operation is performed.
   */
  Color& operator+=(const Color& color);
  /**
   * Substract Operator
   * Substracts each channel respectively.
   * @param color The color to be substracted. Results are clamped between 0.0f and 1.0f.
   * @return Returns the Color object after the substract operation is performed.
   * @see operator+(const Color& c1,const Color& c2)
   */
  Color& operator-=(const Color& color);
  /**
   * Equality Operator
   * Checks if each component is equal to the one in the other color.
   * @param color The color to be compared.
   * @return Returns true if the colors are the same, false otherwise.
   * @see operator-(const Color& c1,const Color& c2);
   */
  bool operator==(const Color& color) const;
  /**
   * Get a C array of float values.
   * C array of float values wrapped in a std::unique_ptr for resource managment.
   * @return Returns a C array of floats wrapped in std::unique_ptr<float[]>.
   */
  std::unique_ptr<float[]> getAsArray() const;
  /**
   * Get a specific channel.
   * Gets a float value between 0.0f and 1.0f from a specific channel.
   * @param component A char element that can take the values: 'r','g','b','a'.
   * @return Returns the value of the channel specified. 0.0f if the value is invalid.
   */
  float getComponent(const char component) const;
};
/**
 * Addition operator.
 * Adds two color objects into a new one.
 * @param c1 First color.
 * @param c2 Second color.
 * @return Returns a new object containing the result of the addition operation.
 */
Color operator+(const Color& c1,const Color& c2);
/**
 * Substraction operator.
 * Substracts two color objects into a new one.
 * @param c1 First color.
 * @param c2 Second color.
 * @return Returns a new object containing the result of the substracton operation.
 */
Color operator-(const Color& c1,const Color& c2);
}

#endif