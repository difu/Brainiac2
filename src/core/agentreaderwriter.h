#ifndef AGENTREADERWRITER_H
#define AGENTREADERWRITER_H

#include <QObject>
#include "src/core/agent.h"

class AgentReaderWriter : public QObject
{
    Q_OBJECT
public:
    enum ConfigBlockType { UNKNOWN, FUZZY, SEGMENT };

    struct ConfigBlock {
        ConfigBlockType type = AgentReaderWriter::UNKNOWN;
        BrainiacGlobals::ItemType itemType = BrainiacGlobals::UNKNOWN;
        QStringList lines;
    };

    explicit AgentReaderWriter(Agent *parent);

    /**
     * Loads the agent from a BAF file.
     *
     * This method reads agent data from a Brainiac Agent File (BAF) format. The BAF format is a plain ASCII based format
     * that is human readable and easily editable. It can be understood by external tools or scripts as well.
     *
     * Before loading, this method checks if the agent has any bones or fuzzies. A new agent can only be loaded if it is empty,
     * i.e., if it doesn't have any bones or fuzzies. If the agent has bones or fuzzies, an error message will be logged and
     * loading will be aborted.
     *
     * The method opens the BAF file in read-only mode using QFile and creates a QTextStream to read the contents of the file.
     * It then iterates over each line in the file and processes the fields in each line. If a field with the value "segment"
     * is found, a new ConfigBlock is created with the type set to ConfigBlockType::SEGMENT. If a field with the value "endSegment"
     * is found, the current ConfigBlock is dispatched and a new one is created. The lines of the ConfigBlock are appended to
     * the ConfigBlock and the type is set. Once all lines are processed, the file is closed and the method returns whether the
     * loading was successful or not.
     *
     * @return true if the agent was successfully loaded, false otherwise.
     */
    [[nodiscard]] bool loadFromBAF();

    [[nodiscard]] bool save();

    /**
     * Saves the agent as a BAF file.
     *
     * A BAF file is the "Brainiac Agent File" format. The BAF format is a plain ASCII based format
     * that is human readable and easily editable. It can be understood by external tools or scripts as well.
     * 
     * @return true if the agent was successfully saved, false otherwise.
     */
    [[nodiscard]] bool saveAsBAF() const;

    /**
     * Adds a segment to the agent.
     *
     * This method is used to add a segment to the agent based on the provided ConfigBlock.
     * The ConfigBlock contains information about the segment, such as its type and properties.
     *
     * The method iterates over each line in the ConfigBlock and processes the fields in each line.
     * If a field with the value "segment" is found, it indicates the start of a new segment. The name
     * of the segment is stored in the variable 'segmentName'.
     *
     * If a field with the value "parent" is found, it indicates the parent of the current segment.
     * The parent is used to determine the hierarchical relationship between segments.
     *
     * If fields with the values "translation", "rotation", and "size" are found, they represent the
     * position, orientation, and size of the segment, respectively.
     *
     * After processing all the lines in the ConfigBlock, the method creates a new BoneBox with the
     * specified segment properties. The BoneBox is then added to the agent's body.
     *
     * @param confBlock The ConfigBlock containing information about the segment.
     */
    void addSegment(ConfigBlock &confBlock);

private:
    Agent *m_agent;
    inline static const QString _indent = "    ";

signals:
};

#endif // AGENTREADERWRITER_H
