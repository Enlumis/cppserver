#ifndef DATACONTAINER_H_
# define DATACONTAINER_H_

class DataContainer
{
public:
  typedef void * dataField;

private:
  std::map<const std::string, dataField>	_datas;

public:
  DataContainer()
  {

  }

  virtual ~DataContainer()
  {

  }

  void	setData(const std::string &key, dataField data)
  {
    _datas[key] = data;
  }

  template<typename T>
  T	*getData(const std::string &key)
  {
    std::map<const std::string, dataField>::iterator it = _datas.find(key);
    if (it != _datas.end())
      return static_cast<T *>(it->second);
    return NULL;
  }

private:
  DataContainer(const DataContainer &);
  DataContainer &operator=(const DataContainer &);
};

#endif /* !DATACONTAINER_H_ */
