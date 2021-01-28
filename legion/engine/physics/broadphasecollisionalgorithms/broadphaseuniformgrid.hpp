#pragma once
#include <physics/broadphasecollisionalgorithms/broadphasecollisionalgorithm.hpp>
#include <physics/physics_statics.hpp>

namespace legion::physics
{
    /**@class BroadphaseUniformGrid
     * @brief Implementation of broad-phase collision detection
     * Creates a virtual axis-aligned grid in which it sorts all physics components with colliders.
     * All colliders in 1 cell have a possibility of colliding, and will be checked in narrow-phase 
     */
    class BroadphaseUniformGrid : public BroadPhaseCollisionAlgorithm
    {
    public:
        BroadphaseUniformGrid(math::ivec3 cellSize, size_type emptyCellDestroyThreshold = 0) : m_cellSize(cellSize), m_emptyCellDestroyThreshold(emptyCellDestroyThreshold)
        {
        }

        /**@brief Collects collider pairs that have a chance of colliding and should be checked in narrow-phase collision detection
         * @param manifoldPrecursors all the physics components
         * @param manifoldPrecursorGrouping a list-list of colliders that have a chance of colliding and should be checked
         */
        const std::vector<std::vector<physics_manifold_precursor>>& collectPairs(
            std::vector<physics_manifold_precursor>&& manifoldPrecursors) override;

        const std::vector<std::vector<physics_manifold_precursor>>& reConstruct(
            std::vector<physics_manifold_precursor>&& manifoldPrecursors);

        /**@brief Sets the cell size which will be used for the virtual grid
         */
        void setCellSize(math::ivec3 cellSize)
        {
            // Grid size changes, therefore our cached data is no longer valid
            cellIndices.clear();
            m_collectedEntities.clear();
            m_oldPositions.clear();
            m_groupings.clear();
            m_entityOccupiedCells.clear();

            m_cellSize = cellSize;
            m_emptyCells.clear();
        }

        void debugDraw() override;

    private:
        math::ivec3 m_cellSize;
        size_type m_emptyCellDestroyThreshold;

        /**@brief Calculates a cell index from a point. i.e. calculates in which cell in the uniform grid a point will be. 
         */
        math::ivec3 calculateCellIndex(const math::vec3 point);
        std::unordered_map<math::ivec3, int> cellIndices;

        std::unordered_set<id_type> m_collectedEntities;
        std::unordered_map<id_type, math::vec3> m_oldPositions;
        std::unordered_map<id_type, std::unordered_set<math::ivec3>> m_entityOccupiedCells;
        std::unordered_set<math::ivec3> m_emptyCells;
    };
}
