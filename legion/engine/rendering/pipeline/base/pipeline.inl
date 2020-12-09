#pragma once

namespace legion::rendering
{
    template<typename Self>
    std::multimap<priority_type, std::unique_ptr<RenderStageBase>, std::greater<>> RenderPipeline<Self>::m_stages;


    template<typename T>
    inline bool RenderPipelineBase::has_meta(const std::string& name)
    {
        id_type id = nameHash(name);
        return m_metadata.count(id) && (m_metadata[id].type() == typeid(T));
    }

    template<typename T, typename... Args>
    inline T* RenderPipelineBase::create_meta(const std::string& name, Args&&... args)
    {
        id_type id = nameHash(name);

        if (m_metadata.count(id))
        {
            if (m_metadata[id].type() == typeid(T))
                return std::any_cast<T>(&m_metadata[id]);
            else
                return nullptr;
        }

        m_metadata.emplace(id, std::make_any<T>(std::forward<Args>(args)...));
        return std::any_cast<T>(&m_metadata[id]);
    }

    template<typename T>
    inline T* RenderPipelineBase::get_meta(const std::string& name)
    {
        id_type id = nameHash(name);

        if (m_metadata.count(id) && (m_metadata[id].type() == typeid(T))
            return std::any_cast<T>(&m_metadata[id]);
            return nullptr;
    }

    template<typename T>
    inline bool RenderPipelineBase::has_meta(id_type nameHash)
    {
        return m_metadata.count(nameHash) && (m_metadata[nameHash].type() == typeid(T));
    }

    template<typename T, typename... Args>
    inline T* RenderPipelineBase::create_meta(id_type nameHash, Args&&... args)
    {
        if (m_metadata.count(nameHash))
        {
            if (m_metadata[nameHash].type() == typeid(T))
                return std::any_cast<T>(&m_metadata[nameHash]);
            else
                return nullptr;
        }

        m_metadata.emplace(nameHash, std::make_any<T>(std::forward<Args>(args)...);
        return std::any_cast<T>(&m_metadata[nameHash]);
    }

    template<typename T>
    inline T* RenderPipelineBase::get_meta(id_type nameHash)
    {
        if (m_metadata.count(nameHash) && (m_metadata[nameHash].type() == typeid(T)))
            return std::any_cast<T>(&m_metadata[nameHash]);
        return nullptr;
    }

    template<typename Self>
    template<typename StageType, inherits_from<StageType, RenderStage<StageType>>>
    inline void RenderPipeline<Self>::attachStage()
    {
        auto ptr = new StageType();
        m_stages.emplace(ptr->priority(), std::unique_ptr<RenderStageBase>(ptr));
    }

    template<typename Self>
    inline void RenderPipeline<Self>::attachStage(std::unique_ptr<RenderStageBase>&& stage)
    {
        m_stages.emplace(stage->priority(), std::forward<std::unique_ptr<RenderStageBase>&&>(stage));
    }

    template<typename Self>
    inline void RenderPipeline<Self>::init(app::window& context)
    {
        setup(context);
        for (auto& [_, stage] : m_stages)
            stage->setup(context);
    }

    template<typename Self>
    inline void RenderPipeline<Self>::render(app::window& context, camera& cam, const camera::camera_input& camInput, time::span deltaTime)
    {
        m_abort = false;
        for (auto& [_, stage] : m_stages)
        {
            stage->render(context, cam, camInput, deltaTime);
            if (m_abort)
                break;
        }
    }
}
