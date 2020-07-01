#ifndef G4E_PLUGINCONTEXT_HH
#define G4E_PLUGINCONTEXT_HH

#include <vector>
#include <JANA/Services/JServiceLocator.h>

class JFactoryGenerator;

namespace g4e {
    class PluginContext: public JService {
    public:
        explicit PluginContext(JServiceLocator& serviceLocator):
                mServiceLocator(serviceLocator)
        {
        }

        void Add(JFactoryGenerator* factory_generator) {
            mFactoryGenerators.push_back(factory_generator);
        }

        /// Use this in EventSources, Factories, or EventProcessors. Do not call this
        /// from InitPlugin(), as not all JServices may have been loaded yet.
        /// When initializing a Service, use acquire_services() instead.
        template <typename T>
        std::shared_ptr<T> GetService();

        /// Call this from InitPlugin.
        template <typename T>
        void ProvideService(std::shared_ptr<T> service);

    private:

        // Service locator
        JServiceLocator& mServiceLocator;

        // Jana factory generators collection
        std::vector<JFactoryGenerator*> mFactoryGenerators;
    };

    /// A convenience method which delegates to JServiceLocator
    template <typename T>
    std::shared_ptr<T> PluginContext::GetService() {
        return mServiceLocator.get<T>();
    }

/// A convenience method which delegates to JServiceLocator
    template <typename T>
    void PluginContext::ProvideService(std::shared_ptr<T> service) {
        mServiceLocator.provide(service);
    }
}

#endif //G4E_PLUGINCONTEXT_HH
