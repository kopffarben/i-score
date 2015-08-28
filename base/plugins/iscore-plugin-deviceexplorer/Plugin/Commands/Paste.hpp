#pragma once

#include <iscore/command/SerializableCommand.hpp>
#include <iscore/tools/ModelPath.hpp>

#include "Panel/DeviceExplorerModel.hpp"


namespace DeviceExplorer
{
    namespace Command
    {

        class Paste : public iscore::SerializableCommand
        {
                ISCORE_COMMAND_DECL("Paste", "Paste")
                public:
                    ISCORE_SERIALIZABLE_COMMAND_DEFAULT_CTOR(Paste, "DeviceExplorerControl")

                Paste(const iscore::NodePath& parentPath, int row,
                         const QString& text,
                         Path<DeviceExplorerModel>&& Path);


                virtual void undo() override;
                virtual void redo() override;

            protected:
                virtual void serializeImpl(QDataStream&) const override;
                virtual void deserializeImpl(QDataStream&) override;



            protected:
                Path<DeviceExplorerModel> m_model{};
                QByteArray m_data;
                iscore::NodePath m_parentPath;
                int m_row{};
        };
    }
}
