/* Copyright 2013-2021 MultiMC Contributors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include <QDateTime>
#include <QFileInfo>
#include <QList>
#include <memory>

#include "ModDetails.h"
#include "modplatform/packwiz/Packwiz.h"

class Mod
{
public:
    enum ModType
    {
        MOD_UNKNOWN,    //!< Indicates an unspecified mod type.
        MOD_ZIPFILE,    //!< The mod is a zip file containing the mod's class files.
        MOD_SINGLEFILE, //!< The mod is a single file (not a zip file).
        MOD_FOLDER,     //!< The mod is in a folder on the filesystem.
        MOD_LITEMOD,    //!< The mod is a litemod
    };

    Mod() = default;
    Mod(const QFileInfo &file);
    explicit Mod(const QDir& mods_dir, const Packwiz::Mod& metadata);

    QFileInfo filename()        const { return m_file; }
    QDateTime dateTimeChanged() const { return m_changedDateTime; }
    QString   internal_id()     const { return m_internal_id; }
    ModType   type()            const { return m_type; }
    bool      fromMetadata()    const { return m_from_metadata; }
    bool      enabled()         const { return m_enabled; }

    bool valid() const { return m_type != MOD_UNKNOWN; }

    const ModDetails& details() const;
    QString name()        const;
    QString version()     const;
    QString homeurl()     const;
    QString description() const;
    QStringList authors() const;

    bool enable(bool value);

    // delete all the files of this mod
    bool destroy(QDir& index_dir);

    // change the mod's filesystem path (used by mod lists for *MAGIC* purposes)
    void repath(const QFileInfo &file);

    bool shouldResolve()    const { return !m_resolving && !m_resolved; }
    bool isResolving()      const { return m_resolving; }
    int  resolutionTicket() const { return m_resolutionTicket; }

    void setResolving(bool resolving, int resolutionTicket) {
        m_resolving = resolving;
        m_resolutionTicket = resolutionTicket;
    }
    void finishResolvingWithDetails(std::shared_ptr<ModDetails> details){
        m_resolving = false;
        m_resolved = true;
        m_localDetails = details;
    }

protected:
    QFileInfo m_file;
    QDateTime m_changedDateTime;

    QString m_internal_id;
    QString m_name;
    ModType m_type = MOD_UNKNOWN;
    bool m_from_metadata = false;
    std::shared_ptr<ModDetails> m_localDetails;

    bool m_enabled = true;
    bool m_resolving = false;
    bool m_resolved = false;
    int m_resolutionTicket = 0;
};
