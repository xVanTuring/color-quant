#ifndef VBOX_3D_H
#define VBOX_3D_H 1
#include <memory>
#include <queue>
struct Box3d {
  size_t sort_param; /* parameter on which to sort the vbox */
  size_t n_pix;      /* number of pixels in the vbox        */
  size_t vol;        /* quantized volume of vbox            */
  int r1;            /* min r index in the vbox             */
  int r2;            /* max r index in the vbox             */
  int g1;            /* min g index in the vbox             */
  int g2;            /* max g index in the vbox             */
  int b1;            /* min b index in the vbox             */
  int b2;            /* max b index in the vbox             */
};

// typedef struct Box3d BOX3D;
class Compare {
public:
  bool operator()(std::shared_ptr<Box3d> b1, std::shared_ptr<Box3d> b2) {
    return b1->sort_param <= b2->sort_param;
    ;
  }
};
typedef std::priority_queue<std::shared_ptr<Box3d>,
                            std::vector<std::shared_ptr<Box3d>>, Compare>
    Box3dHeap;
std::shared_ptr<Box3d> box_3d_create(int r1, int r2, int g1, int g2, int b1,
                                     int b2);

std::shared_ptr<Box3d> box_3d_copy(std::shared_ptr<Box3d> vbox);

size_t vbox_get_count(std::shared_ptr<Box3d> vbox, std::shared_ptr<int> histo,
                      int sigbits);

size_t vbox_get_volume(std::shared_ptr<Box3d> vbox);
void vbox_get_average_color(std::shared_ptr<Box3d> vbox,
                            std::shared_ptr<int> histo, int sigbits,
                            int index, int &p_rval, int &p_gval, int &p_bval);

#endif
