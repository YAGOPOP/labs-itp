#include <cstddef>
#include <string>

using std::string;
using price_t = unsigned int;

// вар 9
//
// Товар, велосипед, горный велосипед, самокат

class Product {
  private:
    price_t price;
    string product_name;
    long article_number;
    string manufacturer;

  public:
    Product(
        price_t price,
        string product_name,
        long article_number,
        string manufacturer
    )
        : price(price), product_name(product_name),
          article_number(article_number), manufacturer(manufacturer) {}

    price_t get_price() { return price; }
    string get_product_name() { return product_name; }
    long get_article_number() { return article_number; }
    string get_manufacturer() { return manufacturer; }
};

class Bicycle : public Product {
  private:
    unsigned long frame_size;
    size_t gear_count;
    bool suspension;

  public:
    Bicycle(
        price_t price,
        string product_name,
        long article_number,
        string manufacturer,
        size_t frame_size,
        size_t gear_count,
        bool suspension
    )
        : Product(price, product_name, article_number, manufacturer),
          frame_size(frame_size), gear_count(gear_count),
          suspension(suspension) {}

    unsigned long get_frame_size() { return frame_size; }
    size_t get_gear_count() { return gear_count; }
    bool has_suspension() { return suspension; }
};

class MountainBicycle : public Bicycle {
  private:
    unsigned long tire_width;

  public:
    MountainBicycle(
        price_t price,
        string product_name,
        long article_number,
        string manufacturer,
        size_t frame_size,
        size_t gear_count,
        bool has_suspension,
        unsigned long tire_width
    )
        : Bicycle(
              price,
              product_name,
              article_number,
              manufacturer,
              frame_size,
              gear_count,
              has_suspension
          ),
          tire_width(tire_width) {}

    unsigned long get_tire_width() { return tire_width; }
};

class Scooter : public Product {
  private:
    unsigned long max_load;
    bool foldable;

  public:
    Scooter(
        price_t price,
        string product_name,
        long article_number,
        string manufacturer,
        unsigned long max_load,
        bool foldable
    )
        : Product(price, product_name, article_number, manufacturer),
          max_load(max_load), foldable(foldable) {}

    unsigned long get_max_load() { return max_load; }
    bool is_foldable() { return foldable; }
};
