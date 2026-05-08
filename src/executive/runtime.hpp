#pragma once

/**
 * @brief Executes all global C++ constructors.
 * 
 * This must be called exactly ONCE before any C++ objects are used.
 * It iterates through the .init_array section and fires the constructors 
 * for all statically allocated objects.
 */
void cpp_runtime_init();