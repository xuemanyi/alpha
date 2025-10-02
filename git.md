## git push
1. 推送到`origin`远程仓库的当前分支
    `git push` 等同于`git push origin HEAD`
2. 推送到指定分支
    - 将当前分支推送到`origin`的`master`分支  
        `git push origin master`
    - 将本地`feature`分支推送到`origin`的`feature`分支  
        `git push origin feature`
    - 本地分支与远程分支不同名时  
        `git push origin local-branch:remote-branch`
3. 设置上游分支（跟踪分支）
    - 第一次推送时设置上游分支  
        `git push -u origin master`  
        之后就可以直接使用`git push`，或者使用`--set-upstream`  
        `git push --set-upstream origin feature`
4. 强制推送（谨慎使用）
    - 强制覆盖远程分支  
        `git push --force origin master`
    - 更安全的强制推送（推荐）  
        `git push --force-with-lease origin master`
## git pull
1. 从默认远程仓库拉取
    - 从上游分支拉取并合并到当前分支  
        `git pull`  
        等同于`git pull origin HEAD`
2. 从指定分支拉取
    - 从origin的master分支拉取到当前分支
        `git pull origin master`
    - 从origin的develop分支拉取到当前分支
        `git pull origin develop`
    - 指定远程分支和本地分支
        `git pull origin remote-branch:local-branch`
## 常见commit信息前缀
| 前缀         | 用途           | 示例                 |
|-------------|---------------|---------------------|
| `feat:`     | 新功能         | `feat: 添加支付功能` |
| `fix:`      | bug修复        | `fix: 修复登录失败问题` |
| `docs:`     | 文档更新       | `docs: 更新部署文档` |
| `style:`    | 代码样式       | `style: 调整CSS变量命名` |
| `refactor:` | 重构           | `refactor: 优化数据库查询` |
| `perf:`     | 性能优化       | `perf: 减少页面加载时间` |
| `test:`     | 测试相关       | `test: 添加单元测试用例` |
| `chore:`    | 构建/工具      | `chore: 更新依赖包版本` |
| `build:`    | 构建系统       | `build: 配置CI/CD流水线` |
| `ci:`       | CI配置         | `ci: 添加GitHub Actions` |
| `revert:`   | 回滚提交       | `revert: 回滚某次错误提交` |
## 常见分支前缀名

| 前缀 | 用途 | 示例 |
|:---|:---|:---|
| `feature/` | 新功能开发 | `feature/search-function` |
| `bugfix/` | 问题修复 | `bugfix/memory-leak` |
| `hotfix/` | 紧急生产环境修复 | `hotfix/security-patch` |
| `release/` | 版本发布准备 | `release/v2.1.0` |
| `experiment/` | 实验性功能 | `experiment/new-ui` |
