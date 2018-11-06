pipeline {
  agent any
  environment {
    CRED = credentials('supsi-scm-denvit')
  }
  stages {
    stage("Slack Noti"){
      steps {
        slackSend color: '#2c3e50', message: "Started build <${BUILD_URL}|#${BUILD_NUMBER}> for ${JOB_NAME} (<https://git.ded1.denv.it/shrug/tuna-ge/commit/${GIT_COMMIT}|${GIT_COMMIT}>) on branch $GIT_BRANCH."
      }
    }
    stage("Slack Notification"){
      steps {
        slackSend color: '#2c3e50', message: "Started build <${BUILD_URL}|#${BUILD_NUMBER}> for ${JOB_NAME} (<https://git.ded1.denv.it/shrug/tuna-ge/commit/${GIT_COMMIT}|${GIT_COMMIT}>) on branch $GIT_BRANCH."
      }
    }
  }
  post {
    failure {
        slackSend color: 'danger', message: "Build <${BUILD_URL}|#${BUILD_NUMBER}> *failed*! Branch $GIT_BRANCH, commit: (<https://git.ded1.denv.it/shrug/tuna-ge/commit/${GIT_COMMIT}|${GIT_COMMIT}>). :respects:\n\n*Commit Log*:\n${COMMIT_LOG}"
    }
    success {
        slackSend color: 'good', message: "Build <${BUILD_URL}|#${BUILD_NUMBER}> *successful*! Branch $GIT_BRANCH, commit: (<https://git.ded1.denv.it/shrug/tuna-ge/commit/${GIT_COMMIT}|${GIT_COMMIT}>). :tada: :blobdance: :clappa:\n\n*Commit Log*:\n${COMMIT_LOG}"
    }
    always {
        script {
          try {
          COMMIT_LOG = sh(script:"git log --oneline --pretty=format:'%h - %s (%an)' ${GIT_PREVIOUS_COMMIT}..HEAD", returnStdout: true)
          } catch(e) {
          COMMIT_LOG = ""
          }
        }
        sh "git remote add supsi https://$CRED@scm.ti-edu.ch/repogit/labingsw022018201907tunagelibrary.git || true"
        sh "git push -u supsi origin/$GIT_BRANCH:$GIT_BRANCH"
        sh "git push --tags supsi"
        cleanWs()
    }
  }
}
