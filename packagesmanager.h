#ifndef PACKAGESMANAGER_H
#define PACKAGESMANAGER_H

#include "result.h"

#include <QObject>
#include <QFuture>

#include <QApt/Backend>
#include <QApt/DebFile>

typedef Result<QString> ConflictResult;

class PackageDependsStatus
{
public:
    static PackageDependsStatus ok();
    static PackageDependsStatus available();
    static PackageDependsStatus _break(const QString &package);

    PackageDependsStatus();
    PackageDependsStatus(const int status, const QString &package);
    PackageDependsStatus operator =(const PackageDependsStatus &other);

    PackageDependsStatus max(const PackageDependsStatus &other);
    PackageDependsStatus maxEq(const PackageDependsStatus &other);
    PackageDependsStatus min(const PackageDependsStatus &other);
    PackageDependsStatus minEq(const PackageDependsStatus &other);

    bool isBreak() const;
    bool isAvailable() const;

public:
    int status;
    QString package;
};

class DebListModel;
class PackagesManager : public QObject
{
    Q_OBJECT

    friend class DebListModel;

public:
    explicit PackagesManager(QObject *parent = 0);

    bool isBackendReady();
    const ConflictResult packageConflictStat(const int index);
    const ConflictResult isConflictSatisfy(const QString &arch, QApt::Package *package);
    const ConflictResult isConflictSatisfy(const QString &arch, const QList<QApt::DependencyItem> &conflicts);
    int packageInstallStatus(const int index);
    PackageDependsStatus packageDependsStatus(const int index);
    const QString packageInstalledVersion(const int index);
    const QStringList packageAvailableDepends(const int index);
    const QSet<QString> packageCandidateChoose(const QString &debArch, const QList<QApt::DependencyItem> &dependsList);
    void packageCandidateChoose(QSet<QString> &choosed_set, const QString &debArch, const QApt::DependencyItem &candidateItem);
    const QStringList packageReverseDependsList(const QString &packageName, const QString &sysArch);

    void resetPackageDependsStatus(const int index);

    QApt::DebFile * const package(const int index) const { return m_preparedPackages[index]; }
    QApt::Backend * const backend() const { return m_backendFuture.result(); }

private:
    const PackageDependsStatus checkDependsPackageStatus(const QString &architecture, const QApt::DependencyInfo &dependencyInfo);
    QApt::Package * packageWithArch(const QString &packageName, const QString &sysArch, const QString &annotation = QString());

private:
    QFuture<QApt::Backend *> m_backendFuture;
    QList<QApt::DebFile *> m_preparedPackages;
    QHash<int, int> m_packageInstallStatus;
    QHash<int, PackageDependsStatus> m_packageDependsStatus;
};

#endif // PACKAGESMANAGER_H
