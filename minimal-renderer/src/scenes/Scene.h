#pragma once

class Renderer;

namespace scene {

	class Scene
	{
	protected:
		Renderer& m_renderer;
	public:
		Scene(Renderer& in_renderer) : m_renderer(in_renderer) {}
		virtual ~Scene(){}

		virtual void OnUpdate(double delta_time) {}
		virtual void OnRender() {}
		virtual void OnImGuiRender() {}
	};
}
