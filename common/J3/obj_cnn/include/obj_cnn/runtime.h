// Copyright 2021 MINIEYE

#ifndef INCLUDE_OBJ_CNN_RUNTIME_H_
#define INCLUDE_OBJ_CNN_RUNTIME_H_

namespace obj_cnn {

/** Setup function of runtime of obj_cnn.
 *
 * \return 0 if succeed.
 */
int RuntimeInitialize();

/** Clean up runtime.
 *
 * \return 0 if succeed.
 */
int RuntimeClean();

}  // namespace obj_cnn

#endif  // INCLUDE_OBJ_CNN_RUNTIME_H_
