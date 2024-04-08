#ifndef BOX__H
#define BOX__H

#include <bitset>
#include <mutex>
#include <shared_mutex>
#include <vector>
#include <unordered_set>
#include <utility>

class Box{

public:
    Box(int id, int group, int width, int height);
    Box() = default;
    Box(const Box& o);
    Box(Box&& o) noexcept;
    Box& operator= (const Box& o) = delete;
    Box& operator= (Box&& o) noexcept = delete;
    ~Box() noexcept = default;

    void upLevel();
    void setId(int id);

    // TODO test setGroupid
    void setGroupid(int groupid);
    void setWidth(int w);
    void setHeight(int h);

    int getId() const;    
    int getGroupId() const;
    int getWidth() const;
    int getHeight() const;
    int getLevel() const;
    
    bool operator== (const Box& o) const;

private:
    int _id     = -1;
    int _groupid = -1;
    int _level  = 0;
    int _width  = -1; 
    int _height = -1;
    mutable std::shared_mutex _mm;
};

// TODO may need to update this with groupid
namespace std
{
    template<>
    struct hash<Box>
    {
        size_t operator()(const Box& b) const
        {   return (
                     ( 
                       ( 
                         ( 
                         hash<int>()(b.getId()) ^ ( hash<int>()(b.getWidth()) << 1)
                         ) >> 1 
                       ) ^ (hash<int>()(b.getHeight()) << 1)
                     ) >> 1);
        }
    };
}

#endif
