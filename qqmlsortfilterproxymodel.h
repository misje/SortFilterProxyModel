#ifndef QQMLSORTFILTERPROXYMODEL_H
#define QQMLSORTFILTERPROXYMODEL_H

#include <QSortFilterProxyModel>
#include <QQmlParserStatus>
#include <QQmlListProperty>

namespace qqsfpm {

class Filter;
class Sorter;
class ProxyRole;

class QQmlSortFilterProxyModel : public QSortFilterProxyModel, public QQmlParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)

    Q_PROPERTY(int count READ count NOTIFY countChanged)

    Q_PROPERTY(QString filterRoleName READ filterRoleName WRITE setFilterRoleName NOTIFY filterRoleNameChanged)
    Q_PROPERTY(QString filterPattern READ filterPattern WRITE setFilterPattern NOTIFY filterPatternChanged)
    Q_PROPERTY(PatternSyntax filterPatternSyntax READ filterPatternSyntax WRITE setFilterPatternSyntax NOTIFY filterPatternSyntaxChanged)
    Q_PROPERTY(QVariant filterValue READ filterValue WRITE setFilterValue NOTIFY filterValueChanged)

    Q_PROPERTY(QString sortRoleName READ sortRoleName WRITE setSortRoleName NOTIFY sortRoleNameChanged)
    Q_PROPERTY(bool ascendingSortOrder READ ascendingSortOrder WRITE setAscendingSortOrder NOTIFY ascendingSortOrderChanged)

    Q_PROPERTY(QQmlListProperty<qqsfpm::Filter> filters READ filters)
    Q_PROPERTY(QQmlListProperty<qqsfpm::Sorter> sorters READ sorters)
    Q_PROPERTY(QQmlListProperty<qqsfpm::ProxyRole> proxyRoles READ proxyRoles)

public:
    enum PatternSyntax {
        RegExp = QRegExp::RegExp,
        Wildcard = QRegExp::Wildcard,
        FixedString = QRegExp::FixedString,
        RegExp2 = QRegExp::RegExp2,
        WildcardUnix = QRegExp::WildcardUnix,
        W3CXmlSchema11 = QRegExp::W3CXmlSchema11 };
    Q_ENUMS(PatternSyntax)

    QQmlSortFilterProxyModel(QObject* parent = 0);

    int count() const;

    const QString& filterRoleName() const;
    void setFilterRoleName(const QString& filterRoleName);

    QString filterPattern() const;
    void setFilterPattern(const QString& filterPattern);

    PatternSyntax filterPatternSyntax() const;
    void setFilterPatternSyntax(PatternSyntax patternSyntax);

    const QVariant& filterValue() const;
    void setFilterValue(const QVariant& filterValue);

    const QString& sortRoleName() const;
    void setSortRoleName(const QString& sortRoleName);

    bool ascendingSortOrder() const;
    void setAscendingSortOrder(bool ascendingSortOrder);

    QQmlListProperty<Filter> filters();
    QQmlListProperty<Sorter> sorters();
    QQmlListProperty<ProxyRole> proxyRoles();

    void classBegin() override;
    void componentComplete() override;

    QVariant sourceData(const QModelIndex& sourceIndex, const QString& roleName) const;
    QVariant sourceData(const QModelIndex& sourceIndex, int role) const;

    QVariant data(const QModelIndex& index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE int roleForName(const QString& roleName) const;

    Q_INVOKABLE QVariantMap get(int row) const;
    Q_INVOKABLE QVariant get(int row, const QString& roleName) const;

    Q_INVOKABLE QModelIndex mapToSource(const QModelIndex& proxyIndex) const override;
    Q_INVOKABLE int mapToSource(int proxyRow) const;
    Q_INVOKABLE QModelIndex mapFromSource(const QModelIndex& sourceIndex) const override;
    Q_INVOKABLE int mapFromSource(int sourceRow) const;

    void setSourceModel(QAbstractItemModel *sourceModel) override;

    static void registerTypes();

public Q_SLOTS:
    void sort(int column, Qt::SortOrder order = Qt::AscendingOrder) override;

Q_SIGNALS:
    void countChanged();

    void filterRoleNameChanged();
    void filterPatternSyntaxChanged();
    void filterPatternChanged();
    void filterValueChanged();

    void sortRoleNameChanged();
    void ascendingSortOrderChanged();

protected:
    bool filterAcceptsRow(int source_row, const QModelIndex& source_parent) const override;
    bool lessThan(const QModelIndex& source_left, const QModelIndex& source_right) const override;

protected Q_SLOTS:
    void resetInternalData();

private Q_SLOTS:
    void invalidateFilter();
    void invalidate();
    void updateRoleNames();
    void updateFilterRole();
    void updateSortRole();
    void updateRoles();
    void initRoles();
    void onDataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight, const QVector<int>& roles);
    void emitProxyRolesChanged();

private:
    QVariantMap modelDataMap(const QModelIndex& modelIndex) const;

    static void append_filter(QQmlListProperty<Filter>* list, Filter* filter);
    static int count_filter(QQmlListProperty<Filter>* list);
    static Filter* at_filter(QQmlListProperty<Filter>* list, int index);
    static void clear_filters(QQmlListProperty<Filter>* list);

    static void append_sorter(QQmlListProperty<Sorter>* list, Sorter* sorter);
    static int count_sorter(QQmlListProperty<Sorter>* list);
    static Sorter* at_sorter(QQmlListProperty<Sorter>* list, int index);
    static void clear_sorters(QQmlListProperty<Sorter>* list);

    static void append_proxyRole(QQmlListProperty<ProxyRole>* list, ProxyRole* proxyRole);
    static int count_proxyRole(QQmlListProperty<ProxyRole>* list);
    static ProxyRole* at_proxyRole(QQmlListProperty<ProxyRole>* list, int index);
    static void clear_proxyRoles(QQmlListProperty<ProxyRole>* list);

    QString m_filterRoleName;
    QVariant m_filterValue;
    QString m_sortRoleName;
    bool m_ascendingSortOrder = true;
    QList<Filter*> m_filters;
    QList<Sorter*> m_sorters;
    QList<ProxyRole*> m_proxyRoles;
    bool m_completed = false;
    QHash<int, QByteArray> m_roleNames;
    QHash<int, ProxyRole*> m_proxyRoleMap;
    QVector<int> m_proxyRoleNumbers;
};

}

#endif // QQMLSORTFILTERPROXYMODEL_H
